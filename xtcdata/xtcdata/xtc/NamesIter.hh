#include "xtcdata/xtc/XtcIterator.hh"
#include "xtcdata/xtc/DescData.hh"
#include "xtcdata/xtc/NamesLookup.hh"

namespace XtcData{
class NamesIter : public XtcData::XtcIterator<Xtc>
{
public:
    enum { Stop, Continue };
    NamesIter(Xtc* xtc, const void* bufEnd) : XtcIterator<Xtc>(xtc, bufEnd) {}
    NamesIter() : XtcIterator<Xtc>() {}
    virtual int process(Xtc* xtc, const void* bufEnd);
    NamesLookup& namesLookup() {return _namesLookup;}
private:
    NamesLookup _namesLookup;
};
};
