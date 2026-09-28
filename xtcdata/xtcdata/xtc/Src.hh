#ifndef XtcData_Src_hh
#define XtcData_Src_hh

#include <limits>
#include <stdint.h>
#include "xtcdata/xtc/Level.hh"

namespace XtcData
{

class Src
{
private:
    enum { LevelBitMask = 0xf0000000, LevelBitShift = 28 };
    enum { ValueBitMask = 0x0fffffff };

public:
    Src(Level::Type level=Level::Segment) :
        _value(level<<LevelBitShift) {}
    Src(unsigned value, Level::Type level=Level::Segment) :
        _value((value&ValueBitMask)|((level<<LevelBitShift)&LevelBitMask)) {}

    Level::Type level() const {return (Level::Type)((_value&LevelBitMask)>>LevelBitShift);}
    unsigned    value() const {return _value&ValueBitMask;}

  protected:
    uint32_t _value;
};

class Src_xtc1 {
  public:

    Src_xtc1() : _log(std::numeric_limits<uint32_t>::max()), _phy(std::numeric_limits<uint32_t>::max()) {}
    Src_xtc1(Level_xtc1::Type level) {
        uint32_t temp = (uint32_t)level;
        _log=(temp&0xff)<<24;
    }

    uint32_t log()   const { return _log; }
    uint32_t phy()   const { return _phy; }

    Level_xtc1::Type level() const { return (Level_xtc1::Type)((_log>>24)&0xff); }

    bool operator==(const Src_xtc1& s) const { return _phy==s._phy && _log==s._log; }
    bool operator<(const Src_xtc1& s) const { return (_phy<s._phy) || ((_phy==s._phy) && (_log<s._log)); }

    static uint32_t _sizeof() { return sizeof(Src_xtc1); }
  protected:
    uint32_t _log; // logical  identifier
    uint32_t _phy; // physical identifier
  };

  class DetInfo : public Src_xtc1 {
  public:
    /*
     * Notice: New enum values should be appended to the end of the enum list, since
     *   the old values have already been recorded in the existing xtc files. 
     */
    enum Detector {
      NoDetector              = 0,
      AmoIms                  = 1,
      AmoGasdet               = 2,
      AmoETof                 = 3,
      AmoITof                 = 4,
      AmoMbes                 = 5,
      AmoVmi                  = 6,
      AmoBps                  = 7,
      Camp                    = 8,
      EpicsArch               = 9,
      BldEb                   = 10,
      SxrBeamline             = 11,
      SxrEndstation           = 12,
      XppSb1Ipm               = 13,
      XppSb1Pim               = 14,
      XppMonPim               = 15,
      XppSb2Ipm               = 16,
      XppSb3Ipm               = 17,
      XppSb3Pim               = 18,
      XppSb4Pim               = 19,
      XppGon                  = 20,
      XppLas                  = 21,
      XppEndstation           = 22,
      AmoEndstation           = 23,
      CxiEndstation           = 24,
      XcsEndstation           = 25,
      MecEndstation           = 26,
      CxiDg1                  = 27,
      CxiDg2                  = 28,
      CxiDg3                  = 29,
      CxiDg4                  = 30,
      CxiKb1                  = 31,
      CxiDs1                  = 32,
      CxiDs2                  = 33,
      CxiDsu                  = 34,
      CxiSc1                  = 35,
      CxiDsd                  = 36,
      XcsBeamline             = 37,
      CxiSc2                  = 38,
      MecXuvSpectrometer      = 39,
      MecXrtsForw             = 40,
      MecXrtsBack             = 41,
      MecFdi                  = 42,
      MecTimeTool             = 43,
      MecTargetChamber        = 44,
      FeeHxSpectrometer       = 45,
      XrayTransportDiagnostic = 46,
      Lamp                    = 47,
      MfxEndstation           = 48,
      MfxDg1                  = 49,
      MfxDg2                  = 50,
      XrtDiag                 = 51,
      DetLab                  = 52,
      NumDetector             = 53
    };

    enum Device {
      NoDevice          = 0,
      Evr               = 1,
      Acqiris           = 2,
      Opal1000          = 3,
      TM6740            = 4,
      pnCCD             = 5,
      Princeton         = 6,
      Fccd              = 7,
      Ipimb             = 8,
      Encoder           = 9,
      Cspad             = 10,
      AcqTDC            = 11,
      Xamps             = 12,
      Cspad2x2          = 13,
      Fexamp            = 14,
      Gsc16ai           = 15,
      Phasics           = 16,
      Timepix           = 17,
      Opal2000          = 18,
      Opal4000          = 19,
      OceanOptics       = 20,
      Opal1600          = 21,
      Opal8000          = 22,
      Fli               = 23,
      Quartz4A150       = 24,
      Andor             = 25,
      USDUSB            = 26,
      OrcaFl40          = 27,
      Imp               = 28,
      Epix              = 29,
      Rayonix           = 30,
      EpixSampler       = 31,
      Pimax             = 32,
      Fccd960           = 33,
      Epix10k           = 34,
      Epix100a          = 35,
      EpixS             = 36,
      Gotthard          = 37,
      DualAndor         = 38,
      Wave8             = 39,
      LeCroy            = 40,
      ControlsCamera    = 41,
      Archon            = 42,
      Jungfrau          = 43,
      Zyla              = 44,
      Epix10ka          = 45,
      Pixis             = 46,
      Uxi               = 47,
      Epix10ka2M        = 48,
      StreakC7700       = 49,
      Epix10kaQuad      = 50,
      JungfrauSegment   = 51,
      JungfrauSegmentM2 = 52,
      JungfrauSegmentM3 = 53,
      JungfrauSegmentM4 = 54,
      iStar             = 55,
      QuadAdc           = 56,
      Alvium            = 57,
      NumDevice = 58
    };

    DetInfo() {}
    DetInfo(uint32_t processId, Detector det, uint32_t detId, Device dev, uint32_t devId);
    DetInfo(const char*);

    bool operator==(const DetInfo &) const;
    bool operator<(const DetInfo &) const;

    uint32_t processId() const;
    Detector detector() const;
    Device device() const;
    uint32_t detId() const;
    uint32_t devId() const;

    static const char *name(Detector);
    static const char *name(Device);
    static const char *name(const DetInfo &);
  };

  class BldInfo : public Src_xtc1 {
  public:

    enum Type { EBeam            = 0,   // Global
                PhaseCavity      = 1,
                FEEGasDetEnergy  = 2,
                Nh2Sb1Ipm01      = 3,   // XPP + downstream
                HxxUm6Imb01      = 4,   // XRT
                HxxUm6Imb02      = 5,
                HfxDg2Imb01      = 6,
                HfxDg2Imb02      = 7,
                XcsDg3Imb03      = 8,
                XcsDg3Imb04      = 9,
                HfxDg3Imb01      = 10,
                HfxDg3Imb02      = 11,
                HxxDg1Cam        = 12,
                HfxDg2Cam        = 13,
                HfxDg3Cam        = 14,
                XcsDg3Cam        = 15,
                HfxMonCam        = 16,
                HfxMonImb01      = 17,
                HfxMonImb02      = 18,
                HfxMonImb03      = 19,
                MecLasEm01       = 20,  // MEC Local
                MecTctrPip01     = 21,
                MecTcTrDio01     = 22,
                MecXt2Ipm02      = 23,
                MecXt2Ipm03      = 24,
                MecHxmIpm01      = 25,
                GMD              = 26,  // SXR Local
                CxiDg1Imb01      = 27,  // CXI Local
                CxiDg2Imb01      = 28,
                CxiDg2Imb02      = 29,
                CxiDg3Imb01      = 30,
                CxiDg1Pim        = 31,
                CxiDg2Pim        = 32,
                CxiDg3Pim        = 33,
                XppMonPim0       = 34,
                XppMonPim1       = 35,
                XppSb2Ipm        = 36,
                XppSb3Ipm        = 37,
                XppSb3Pim        = 38,
                XppSb4Pim        = 39,
                XppEndstation0   = 40,
                XppEndstation1   = 41,
                MecXt2Pim02      = 42,
                MecXt2Pim03      = 43,
                CxiDg3Spec       = 44,
                Nh2Sb1Ipm02      = 45,   // XPP + downstream
                FeeSpec0         = 46,
                SxrSpec0         = 47,
                XppSpec0         = 48,
                XcsUsrIpm01      = 49,
                XcsUsrIpm02      = 50,
                XcsUsrIpm03      = 51,
                XcsUsrIpm04      = 52,
                XcsSb1Ipm01      = 53,
                XcsSb1Ipm02      = 54,
                XcsSb2Ipm01      = 55,
                XcsSb2Ipm02      = 56,
                XcsGonIpm01      = 57,
                XcsLamIpm01      = 58,
                XppAin01         = 59,
                XcsAin01         = 60,
                AmoAin01         = 61,
                MfxBeamMon01     = 62,
                EOrbits          = 63,
                MfxDg1Pim        = 64,
                MfxDg2Pim        = 65,
                SxrAin01         = 66,
                Hx2Sb1BeamMon    = 67,
                XrtUsbEncoder01  = 68,
                XppUsbEncoder01  = 69,
                XppUsbEncoder02  = 70,
                XcsUsbEncoder01  = 71,
                CxiUsbEncoder01  = 72,
                XcsSndDio        = 73,
                MfxUsrDio        = 74,
                XppSb2BeamMon    = 75,
                XppSb3BeamMon    = 76,
                HfxDg2BeamMon    = 77,
                XcsSb1BeamMon    = 78,
                XcsSb2BeamMon    = 79,
                CxiDg2BeamMon    = 80,
                CxiDg3BeamMon    = 81,
                MfxDg1BeamMon    = 82,
                MfxDg2BeamMon    = 83,
                MfxAin01         = 84,
                MecAin01         = 85,
                FeeAin01         = 86,
                MecXt2BeamMon02  = 87,
                MecXt2BeamMon03  = 88,
                XppUsrDio        = 89,
                XppAlcDio        = 90,
                XcsUsrDio        = 91,
                CxiUsrDio        = 92,
                MecUsrDio        = 93,
                MfxUsbEncoder01  = 94,
                HxxDg1BeamMon    = 95,
                EM3L0BeamMon     = 96,
                NumberOf };

    BldInfo() {}
    BldInfo(uint32_t processId,
            Type     type);
    BldInfo(const char*);

    bool operator==(const BldInfo&) const;

    uint32_t processId() const;
    Type     type()  const;

    static const char* name(const BldInfo&);
  };


}

#endif
