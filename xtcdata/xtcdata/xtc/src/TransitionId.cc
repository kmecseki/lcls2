#include "xtcdata/xtc/TransitionId.hh"

using namespace XtcData;

const char* TransitionId::name(TransitionId::Value id)
{
    static const char* _names[] = {
        "ClearReadout",
        "Reset",
        "Configure",
        "Unconfigure",
        "BeginRun",
        "EndRun",
        "BeginStep",
        "EndStep",
        "Enable",
        "Disable",
        "SlowUpdate",
        "Unused_11",
        "L1Accept",
    };
    return (id < TransitionId::NumberOf ? _names[id] : "-Invalid-");

    // Bail on compilation if someone forgets to update this list
    static_assert(sizeof(_names) / sizeof(*_names) == TransitionId::NumberOf,"test message");
};

const char* TransitionId_xtc1::name(TransitionId_xtc1::Value id)
{
  static const char* _names[] = {
    "Unknown",
    "Reset",
    "Map",
    "Unmap",
    "Configure",
    "Unconfigure",
    "BeginRun",
    "EndRun",
    "BeginCalibCycle",
    "EndCalibCycle",
    "Enable",
    "Disable",
    "L1Accept"
  };
  return (id < TransitionId_xtc1::NumberOf ? _names[id] : "-Invalid-");
};
