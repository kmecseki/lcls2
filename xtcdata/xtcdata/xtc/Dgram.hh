#ifndef XtcData_Dgram_hh
#define XtcData_Dgram_hh

#include "TimeStamp.hh"
#include "TransitionId.hh"
#include "Xtc.hh"
#include <stdint.h>

#pragma pack(push,4)

namespace XtcData
{

class TransitionBase {
public:
    enum Type { Event = 0, Occurrence = 1, Marker = 2 };
    enum { NumberOfTypes = 3 };
    TransitionBase() {}
    TransitionBase(Type type_, TransitionId::Value tid_,
                   const TimeStamp& time_, uint32_t env_) :
        time(time_), env((type_<<28)|(tid_<<24)|(env_&0xffffff)) {}
public:
    uint16_t readoutGroups() const { return (env)&0xffff; }
public:
    TimeStamp time;
    uint32_t env;
};

class Transition : public TransitionBase {
public:
    Transition() {}
    Transition(Type type_, TransitionId::Value tid_,
               const TimeStamp& time_, uint32_t env_) :
      TransitionBase(type_, tid_, time_, env_) {}
public:
    unsigned control()            const { return (env>>24)&0xff; }
    TransitionId::Value service() const { return TransitionId::Value(control()&0xf); }
    Type type()                   const { return Type((control()>>4)&0x3); }
    bool isEvent()                const { return service()==TransitionId::L1Accept; }
};

class Dgram : public Transition {
public:
    static const unsigned MaxSize = 0x1000000;
    Dgram() {}
    Dgram(const Transition& transition_) :
        Transition(transition_) { }
    Dgram(const Transition& transition_, const Xtc& xtc_) :
        Transition(transition_), xtc(xtc_)  { }
public:
    Xtc xtc;
};

class Transition_xtc1 {
public:
    Transition_xtc1() {}
public:
    TransitionId_xtc1::Value service() const 
    { 
        enum {v_cntrl   = 0, k_cntrl   = 8};
        enum {v_service = 0, k_service = 4};
        enum {v_seqtype = 4, k_seqtype = 2};
        enum {v_extend  = 7, k_extend  = 1};
  
        enum {m_cntrl   = ((1 << k_cntrl)  -1), s_cntrl   = (m_cntrl   << v_cntrl)};
        enum {m_service = ((1 << k_service)-1), s_service = (m_service << v_service)};
        return TransitionId_xtc1::Value((time.control() >> v_service) & m_service); 
    }
public:
    TimeStamp clock;
    TimeStamp time;
};


class Dgram_xtc1 : public Transition_xtc1 {
public:
    //virtual size_t dgramSize() const override { return sizeof(Dgram_xtc1); }
    //virtual uint32_t getEnv() { return env; }
    //virtual XtcBase* xtcPtr() override { return &xtc; };
    //uint32_t service() const override { return 0; }
    //virtual TimeStamp getTime() override { return time; }

    Dgram_xtc1() {};

    uint32_t env;
    Xtc1 xtc;
};

class L1Dgram : public Dgram {
public:
    // 8 reserved bits.  Perhaps for future trigger lines?
    // b0   - L0Accept
    // b5:1 - L0Tag
    // b6   - L0Raw
    // b7   - L0Reject
    uint16_t reserved() const { return (env>>16)&0xff; }
    bool     keepRaw () const { return (env>>22)&1; }
};

}

#pragma pack(pop)

#endif
