#include "xtcdata/xtc/Partition.hh"

#include <stdlib.h>
#include <string.h>

namespace XtcData {

DetInfo::DetInfo(uint32_t processId,
         Detector det, uint32_t detId,
         Device dev,   uint32_t devId) : Src_xtc1(Level_xtc1::Source) {
  _log |= processId&0x00ffffff;
  _phy = ((det&0xff)<<24) | ((detId&0xff)<<16) | ((dev&0xff)<<8) |(devId&0xff);
}

DetInfo::DetInfo(const char* sname) : Src_xtc1(Level_xtc1::Source)
{
  Detector det = NumDetector;
  Device   dev = NumDevice;
  unsigned detId = 0;
  unsigned devId = 0;
  _phy = ((det&0xff)<<24) | ((detId&0xff)<<16) | ((dev&0xff)<<8) |(devId&0xff);

  for(unsigned i=0; i<NumDetector; i++) {
    const char* dname = name(Detector(i));
    unsigned len = strlen(dname);
    if (strncmp(sname,dname,len)==0 && sname[len]=='-') {
      det = Detector(i);
      const char* ssname = sname+len+1;
      char* endPtr;
      detId = strtoul(ssname,&endPtr,0);
      if (endPtr == ssname || *endPtr!='|') continue;
      ssname = endPtr+1
;
      for(unsigned j=0; j<NumDevice; j++) {
	dname = name(Device(j));
	len   = strlen(dname);
	if (strncmp(ssname,dname,len)==0 && ssname[len]=='-') {
	  dev = Device(j);
	  const char* sdname = ssname+len+1;
	  devId = strtoul(sdname,&endPtr,0);
	  if (endPtr == sdname) continue;
	  
	  _phy = ((det&0xff)<<24) | ((detId&0xff)<<16) | ((dev&0xff)<<8) |(devId&0xff);
	  break;
	}
      }
    }
  }
}

bool DetInfo::operator==(const DetInfo& s) const { return _phy==s._phy; }
bool DetInfo::operator<(const DetInfo& s) const  { return _phy<s._phy; }

uint32_t DetInfo::processId() const { return _log&0xffffff; }

DetInfo::Detector DetInfo::detector() const {return (Detector)((_phy&0xff000000)>>24);}
DetInfo::Device   DetInfo::device()   const {return (Device)((_phy&0xff00)>>8);}
uint32_t          DetInfo::detId()    const {return (_phy&0xff0000)>>16;}
uint32_t          DetInfo::devId()    const {return _phy&0xff;}
    
const char* DetInfo::name(Detector det){
  static const char* _detNames[] = {
    "NoDetector",
    "AmoIMS", "AmoGD", "AmoETOF", "AmoITOF", "AmoMBES", "AmoVMI", "AmoBPS", "Camp",
    "EpicsArch", "BldEb",
    "SxrBeamline", "SxrEndstation",
    "XppSb1Ipm", "XppSb1Pim", "XppMonPim", "XppSb2Ipm", "XppSb3Ipm", "XppSb3Pim", "XppSb4Pim", "XppGon", "XppLas", "XppEndstation",
    "AmoEndstation", "CxiEndstation", "XcsEndstation", "MecEndstation",
    "CxiDg1", "CxiDg2", "CxiDg3", "CxiDg4", "CxiKb1", "CxiDs1", "CxiDs2", "CxiDsu", "CxiSc1", "CxiDsd",
    "XcsBeamline", "CxiSc2",
    "MecXuvSpectrometer","MecXrtsForw","MecXrtsBack","MecFdi","MecTimeTool","MecTargetChamber",
    "FeeHxSpectrometer", "XrayTransportDiagnostic", "Lamp",
    "MfxEndstation", "MfxDg1", "MfxDg2", "XrtDiag", "DetLab"
  };
  return (det < NumDetector ? _detNames[det] : "-Invalid-");
}

const char* DetInfo::name(Device dev) {
  static const char* _devNames[] = {
    "NoDevice",
    "Evr",
    "Acqiris",
    "Opal1000",
    "Tm6740",
    "pnCCD",
    "Princeton",
    "Fccd",
    "Ipimb",
    "Encoder",
    "Cspad",
    "AcqTDC",
    "Xamps",
    "Cspad2x2",
    "Fexamp",
    "Gsc16ai",
    "Phasics",
    "Timepix",
    "Opal2000",
    "Opal4000",
    "OceanOptics",
    "Opal1600",
    "Opal8000",
    "Fli",
    "Quartz4A150",
    "Andor",
    "USDUSB",
    "OrcaFl40",
    "Imp",
    "Epix",
    "Rayonix",
    "EpixSampler",
    "Pimax",
    "Fccd960",
    "Epix10k",
    "Epix100a",
    "EpixS",
    "Gotthard",
    "DualAndor",
    "Wave8",
    "LeCroy",
    "ControlsCamera",
    "Archon",
    "Jungfrau",
    "Zyla",
    "Epix10ka",
    "Pixis",
    "Uxi",
    "Epix10ka2M",
    "StreakC7700",
    "Epix10kaQuad",
    "JungfrauSegment",
    "JungfrauSegmentM2",
    "JungfrauSegmentM3",
    "JungfrauSegmentM4",
    "iStar",
    "QuadAdc",
    "Alvium",
  };
  return (dev < NumDevice ? _devNames[dev] : "-Invalid-");
}

const char* DetInfo::name(const DetInfo& src) {
  const int MaxLength=64;
  static char _name[MaxLength];
  snprintf(_name, MaxLength, "%s-%u|%s-%u",
       name(src.detector()), (unsigned)src.detId(),
       name(src.device  ()), (unsigned)src.devId());
  return _name;
}


BldInfo::BldInfo(uint32_t processId, Type type) : Src_xtc1(Level_xtc1::Reporter) {
  _log |= processId&0x00ffffff;
  _phy = type;
}

BldInfo::BldInfo(const char* sname) : Src_xtc1(Level_xtc1::Reporter)
{
  for(unsigned i=0; i<NumberOf; i++) {
    _phy = i;
    const char* bname = name(*this);
    unsigned len = strlen(bname);
    if (strncmp(sname,bname,len)==0) {
      return;
    }
  }
  _phy = NumberOf;
}

bool BldInfo::operator==(const BldInfo& o) const
{
  return o.phy()==_phy;
}

uint32_t BldInfo::processId() const { return _log&0xffffff; }

BldInfo::Type BldInfo::type() const {return (BldInfo::Type)(_phy); }

const char* BldInfo::name(const BldInfo& src){
  static const char* _typeNames[] = {
    "EBeam",
    "PhaseCavity",
    "FEEGasDetEnergy",
    "NH2-SB1-IPM-01",
    "XCS-IPM-01",
    "XCS-DIO-01",
    "XCS-IPM-02",
    "XCS-DIO-02",
    "XCS-IPM-03",
    "XCS-DIO-03",
    "XCS-IPM-03m",
    "XCS-DIO-03m",
    "XCS-YAG-1",
    "XCS-YAG-2",
    "XCS-YAG-3m",
    "XCS-YAG-3",
    "XCS-YAG-mono",
    "XCS-IPM-mono",
    "XCS-DIO-mono",
    "XCS-DEC-mono",
    "MEC-LAS-EM-01",
    "MEC-TCTR-PIP-01",
    "MEC-TCTR-DI-01",
    "MEC-XT2-IPM-02",
    "MEC-XT2-IPM-03",
    "MEC-HXM-IPM-01",
    "GMD",
    "CxiDg1_Imb01",
    "CxiDg2_Imb01",
    "CxiDg2_Imb02",
    "CxiDg3_Imb01",
    "CxiDg1_Pim",
    "CxiDg2_Pim",
    "CxiDg3_Pim",
    "XppMon_Pim0",
    "XppMon_Pim1",
    "XppSb2_Ipm",
    "XppSb3_Ipm",
    "XppSb3_Pim",
    "XppSb4_Pim",
    "XppEnds_Ipm0",
    "XppEnds_Ipm1",
    "MEC-XT2-PIM-02",
    "MEC-XT2-PIM-03",
    "CxiDg3_Spec",
    "NH2-SB1-IPM-02",
    "FEE-SPEC0",
    "SXR-SPEC0",
    "XPP-SPEC0",
    "XCS-USR-IPM-01",
    "XCS-USR-IPM-02",
    "XCS-USR-IPM-03",
    "XCS-USR-IPM-04",
    "XCS-IPM-04",
    "XCS-DIO-04",
    "XCS-IPM-05",
    "XCS-DIO-05",
    "XCS-IPM-gon",
    "XCS-IPM-ladm",
    "XPP-AIN-01",
    "XCS-AIN-01",
    "AMO-AIN-01",
    "MFX-BEAMMON-01",
    "EOrbits",
    "MfxDg1_Pim",
    "MfxDg2_Pim",
    "SXR-AIN-01",
    "HX2-SB1-BMMON",
    "XRT-USB-ENCODER-01",
    "XPP-USB-ENCODER-01",
    "XPP-USB-ENCODER-02",
    "XCS-USB-ENCODER-01",
    "CXI-USB-ENCODER-01",
    "XCS-SND-DIO",
    "MFX-USR-DIO",
    "XPP-SB2-BMMON",
    "XPP-SB3-BMMON",
    "HFX-DG2-BMMON",
    "XCS-SB1-BMMON",
    "XCS-SB2-BMMON",
    "CXI-DG2-BMMON",
    "CXI-DG3-BMMON",
    "MFX-DG1-BMMON",
    "MFX-DG2-BMMON",
    "MFX-AIN-01",
    "MEC-AIN-01",
    "FEE-AIN-01",
    "MEC-XT2-BMMON-02",
    "MEC-XT2-BMMON-03",
    "XPP-USR-DIO",
    "XPP-ALC-DIO",
    "XCS-USR-DIO",
    "CXI-USR-DIO",
    "MEC-USR-DIO",
    "MFX-USB-ENCODER-01",
    "HXX-DG1-BMMON-01",
    "EM3L0-BMMON",
  };
  return (src.type() < NumberOf ? _typeNames[src.type()] : "-Invalid-");
}

} // Namespace XtcData
