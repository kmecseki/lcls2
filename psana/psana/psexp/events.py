import time
from .event_manager import EventManager
from psana import dgram

class Events:
    """
    An iterator class for retrieving events from a run.

    Handles different run modes:
    - RunSerial: receives batches from a serial manager (smdr_man)
    - RunParallel: fetches SMD batches using get_smd()
    - RunSingleFile / RunShmem: reads directly from a DgramManager

    This class abstracts the complexity of batching, filtering empty events,
    and respecting termination signals, providing a uniform interface via `__next__()`.
    """
    def __init__(
        self,
        configs,
        dm,
        max_retries,
        use_smds,
        shared_state,
        get_smd=None,
        smdr_man=None,
        on_batch_end=None,
    ):
        self.configs = configs  # Configuration dgrams for event building
        self.dm = dm              # DgramManager for direct reading
        self.max_retries = max_retries  # Max retries for event fetching
        self.use_smds = use_smds  # Flag to indicate SMD usage
        self.shared_state = shared_state  # SimpleNamespace with shared state like terminate_flag
        self.get_smd = get_smd       # Callable to retrieve SMD batches (RunParallel)
        self.smdr_man = smdr_man     # Serial batch manager (RunSerial)
        self._on_batch_end = on_batch_end
        self._evt_man = iter([])     # Current EventManager instance
        self._batch_iter = iter([])  # Iterator over batches for RunSerial
        self._batch_event_count = 0
        self._batch_start_time = None
        self.xtc1 = dm.xtc1

    def __iter__(self):
        return self

    def _is_valid_batch(self, batch_dict):
        return batch_dict and 0 in batch_dict and batch_dict[0]

    def _emit_batch_end(self):
        if not self._on_batch_end:
            return
        if not hasattr(self._evt_man, "get_bd_read_stats"):
            return
        if self._batch_start_time is None:
            return
        elapsed = time.monotonic() - self._batch_start_time
        self._on_batch_end(
            (self._evt_man.get_bd_read_stats(), self._batch_event_count, elapsed)
        )
        self._batch_event_count = 0
        self._batch_start_time = None

    def _multiplex_xtc1_streams(self, raw_views):
        """
        Combines independent XTC1 stream arrays into a single linear byte string
        chronologically synchronized by datagram timestamps.
        """
        import io
        
        cursors = [0] * len(raw_views)
        view_sizes = [memoryview(v).nbytes for v in raw_views]
        
        synthetic_buffer = io.BytesIO()
        stream_identities = []  # Maps each output chunk to its source file index
        event_mappings = []     # Maps each output chunk to a synchronized row index
        
        timestamp_to_event_row = {}
        next_event_row = 0

        while any(cursors[i] < view_sizes[i] for i in range(len(raw_views))):
            current_dgrams = []
            current_timestamps = []

            # Peek at the upcoming datagram for every single active file
            for i_smd in range(len(raw_views)):
                offset = cursors[i_smd]
                if offset >= view_sizes[i_smd]:
                    current_dgrams.append(None)
                    current_timestamps.append(float('inf'))
                    continue

                # Instantiate XTC1 Dgram to check timestamp details
                d = dgram.Dgram_xtc1(config=self.smd_configs[i_smd], view=raw_views[i_smd], offset=offset)
                current_dgrams.append(d)
                current_timestamps.append(d.timestamp())

            # Find the earliest timestamp among all files
            min_ts = min(current_timestamps)
            print("Min timestamp: ", min_ts)
            if min_ts == float('inf'):
                break # Everything is parsed

            # Group concurrent datagrams into a single event row index
            if min_ts not in timestamp_to_event_row:
                timestamp_to_event_row[min_ts] = next_event_row
                next_event_row += 1
            current_row_idx = timestamp_to_event_row[min_ts]

            # Extract and serialize the matching datagram frames
            for i_smd in range(len(raw_views)):
                d = current_dgrams[i_smd]
                if d is not None and current_timestamps[i_smd] == min_ts:
                    offset = cursors[i_smd]

                    # Fetch raw slice out of the original stream chunk
                    dgram_bytes = raw_views[i_smd][offset : offset + d._size]
                    synthetic_buffer.write(dgram_bytes)

                    # Record tracking details for _get_offset_and_size
                    stream_identities.append(i_smd)
                    event_mappings.append(current_row_idx)

                    cursors[i_smd] += d._size

        return synthetic_buffer.getvalue(), stream_identities, event_mappings

    def __next__(self):
        """
        Retrieve the next valid event, skipping empty ones.

        Raises:
            StopIteration: When the data source is exhausted or termination is requested.
        """
        if self.smdr_man:
            # RunSerial: iterate over batches, skipping empty ones
            cn = 0
            while True:
                if not self.xtc1:
                    if self.shared_state.terminate_flag.value:
                        raise StopIteration
                try:
                    dgrams = next(self._evt_man)
                    cn += 1
                    #print("We have dgrams? ", dgrams)
                    if not any(dgrams):
                        continue
                    self._batch_event_count += 1
                    #print("Events __next__: returning dgrams: ", dgrams)
                    return dgrams
                except StopIteration:
                    try:
                        #print("Events __next__ in second part")
                        self._emit_batch_end()
                        #print("Calling next on batch iterator!")
                        batch_dict, _ = next(self._batch_iter)
                        #print("BATCH DICT size:", batch_dict[0][1])
                        #if self.xtc1:
                        #    print("Batch dict length:", len(batch_dict))
                        #    print("XTC files length:", len(self.dm.xtc_files))
                        #    raw_xtc1_views = [batch_dict[0][0]]# for smd_id in range(len(self.dm.xtc_files))]
                        #    synthetic_view, stream_identities, event_mappings = self._multiplex_xtc1_streams(raw_xtc1_views)
                        #    self.current_stream_identities = stream_identities
                        #    self.current_event_mappings = event_mappings
                        #    batch_dict = {0: [(synthetic_view, None)]}
                        # Skip empty or malformed batches
                        if not self._is_valid_batch(batch_dict):
                            continue
                        self._evt_man = EventManager(
                            batch_dict[0][0],
                            self.configs,
                            self.dm,
                            self.max_retries,
                            self.use_smds,
                        )
                        self._batch_event_count = 0
                        self._batch_start_time = time.monotonic()
                    except StopIteration:
                        # Refill the batch iterator from the serial batch manager
                        self._batch_iter = next(self.smdr_man)

        elif self.get_smd:
            # RunParallel: fetch batch from get_smd() when needed
            while True:
                try:
                    dgrams = next(self._evt_man)
                    if not any(dgrams):
                        continue
                    self._batch_event_count += 1
                    return dgrams
                except StopIteration:
                    self._emit_batch_end()
                    smd_batch = self.get_smd()
                    if smd_batch == bytearray():
                        raise StopIteration

                    self._evt_man = EventManager(
                        smd_batch,
                        self.configs,
                        self.dm,
                        self.max_retries,
                        self.use_smds,
                    )
                    self._batch_event_count = 0
                    self._batch_start_time = time.monotonic()
        else:
            # RunSingleFile or RunShmem: read directly from the DgramManager
            while True:
                # Checks if users ask to exit
                if not self.xtc1:
                    if self.shared_state.terminate_flag.value:
                        raise StopIteration
                dgrams = next(self.dm)

                if not any(dgrams):
                    continue
                return dgrams
