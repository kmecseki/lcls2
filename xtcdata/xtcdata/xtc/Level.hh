#ifndef PDSLEVEL_HH
#define PDSLEVEL_HH

namespace XtcData
{
class Level
{
public:
    enum Type { Segment, Event, NumberOfLevels };
    static const char* name(Type type);
};

class Level_xtc1 {
public:
  enum Type{Control, Source, Segment, Event, Recorder, Observer, Reporter,
            NumberOfLevels};
  static const char* name(Type type)
{ 
  static const char* _names[] = {
    "Control",
    "Source",
    "Segment",
    "Event",
    "Recorder",
    "Observer",
    "Reporter"
  };
  return (type < NumberOfLevels ? _names[type] : "-Invalid-");
}
};
}

#endif
