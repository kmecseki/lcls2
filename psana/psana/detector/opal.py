
import numpy as np
from psana.detector.opal_base import opal_base, logging
from psana.detector.detector_impl import DetectorImpl
import struct
from psana.detector.UtilsAreaDetector import arr3d_from_dict # For Opal8000 lcls1

logger = logging.getLogger(__name__)

class opal_raw_2_0_0(opal_base):
    def __init__(self, *args, **kwa):
        opal_base.__init__(self, *args, **kwa)
        #self._add_fields() < overrides det.raw.image(...)


class opal_ttfex_2_0_0(opal_base):
    def __init__(self, *args, **kwa):
        opal_base.__init__(self, *args, **kwa)
        self._add_fields()

#  removed some fields
class opal_ttfex_2_1_0(DetectorImpl):
    def __init__(self, *args, **kwa):
        super(opal_ttfex_2_1_0, self).__init__(*args)
        self._add_fields()

class opal_ttfex_2_1_1(opal_ttfex_2_1_0):
    """Algorithm version 2.1.1 - Address potential race condition.

    Note from GD - 2025/10/22:
    We believe there may have been a race condition which could invalidate results
    stored in the FEX. We think it was possible for parallel threads to modify
    the stored FEX results in `m_flt_position` etc, before the write or caput by
    a competing thread could be done. There were no semaphores or other synchronization
    mechanisms guarding writes and reads to/from these shared member attributes.

    To address this possibility we changed the OpalTTFex::analyze function to return
    the results to the caller instead of store them on member attributes.

    This increment in algorithm indicates that this new approach is being used. There is
    no difference in the structure/format of the data from algorithm 2.1.0.
    """
    def __init__(self, *args, **kwa):
        super().__init__(*args, **kwa)

class opal_ttproj_2_0_0(DetectorImpl):
    def __init__(self, *args, **kwa):
        super(opal_ttproj_2_0_0, self).__init__(*args)
        self._add_fields()


class opal_simfex_2_0_0(opal_base):
    def __init__(self, *args, **kwa):
        opal_base.__init__(self, *args, **kwa)
        self._add_fields()


class opal_simfex_2_1_0(DetectorImpl):
    def __init__(self, *args, **kwa):
        opal_base.__init__(self, *args, **kwa)
        self._add_fields()


class opal_ref_2_0_0(opal_base):
    def __init__(self, *args, **kwa):
        opal_base.__init__(self, *args, **kwa)
        self._add_fields()

class xtc1_opal_base(DetectorImpl):
    def __init__(self, *args, **kwa):
        opal_base.__init__(self, *args, **kwa)
        self.configs = {}
        self._add_configs()

    def raw(self, evt):
        key = (self._det_name, self._drp_class_name)
        segs = evt._det_segments[key]
        return arr3d_from_dict({k:v.bytes for k,v in segs.items()}) if len(segs.items())>1 else\
                next(iter(segs.values())).bytes

    def _add_configs(self):
        for config in self._configs:
            seg = getattr(config.software, self._det_name)
            key0 = list(seg.keys())[0]
            seg_dict = getattr(seg[key0], self._drp_class_name)
            if "confbytes" in vars(seg_dict):
                if "FrameFexConfig" in seg_dict.confbytes:
                    pass
                if "Opal1kConfig" in seg_dict.confbytes:
                    offset_and_gain, output_options, defect_pixel_count = struct.unpack("<III", seg_dict.confbytes["Opal1kConfig"][:12])
                    self.configs["black_level"] = offset_and_gain & 0xffff
                    self.configs["gain_percent"] = (offset_and_gain >> 16) & 0xffff
                    Depth = {
                        0: "Eight_bit",
                        1: "Ten_bit",
                        2: "Twelve_bit",
                    }
                    Binning = {
                        0: "x1",
                        1: "x2",
                        2: "x4",
                        3: "x8",
                    }
                    Mirroring = {
                        0: "None",
                        1: "HFlip",
                        2: "VFlip",
                        3: "HVFlip",
                    }
                    depth = output_options & 0xf
                    self.configs["Depth"] = Depth.get(depth, "Unknown")
                    binning = (output_options >> 4) & 0xf
                    self.configs["Binning"] = Binning.get(binning, "Unknown")
                    mirroring = (output_options >> 8) & 0xf
                    self.configs["Mirroring"] = Mirroring.get(mirroring, "Unknown")

class Opal8000_raw_1_0_0(xtc1_opal_base):
    def __init__(self, *args, **kwa):
        xtc1_opal_base.__init__(self, *args, **kwa)

class Opal1000_raw_1_0_0(xtc1_opal_base):
    def __init__(self, *args, **kwa):
        xtc1_opal_base.__init__(self, *args, **kwa)

# EOF
