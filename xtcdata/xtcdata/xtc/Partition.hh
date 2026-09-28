#ifndef XtcData_Partition_hh
#define XtcData_Partition_hh

#include <stdint.h>
#include <algorithm>
#include <vector>
#include <memory>
#include "xtcdata/xtc/TypeId.hh"
#include "xtcdata/xtc/Src.hh"


namespace XtcData {

#pragma pack(push,4)

class Source {
public:
  Source(const Src_xtc1& arg__src, uint32_t arg__group);
  Source() {}
  const Src_xtc1& src() const { return _src; }
  uint32_t group() const { return _group; }
  static uint32_t _sizeof() { return (((((0+(Src_xtc1::_sizeof()))+4)+4)-1)/4)*4; }
private:
  Src_xtc1	_src;
  uint32_t	_group;
};
#pragma pack(pop)

//class ConfigV1 {
//public:
//  enum { TypeId = TypeId_xtc1::Id_PartitionConfig /**< XTC type ID value (from Pds::TypeId class) */ };
//  enum { Version = 1 /**< XTC type version number */ };
//  ConfigV1(uint64_t arg__bldMask, uint32_t arg__numSources, const Source* arg__sources);
//  ConfigV1() {}
//  ConfigV1(const ConfigV1& other) {
//    const char* src = reinterpret_cast<const char*>(&other);
//    std::copy(src, src+other._sizeof(), reinterpret_cast<char*>(this));
//  }
//  ConfigV1& operator=(const ConfigV1& other) {
//    const char* src = reinterpret_cast<const char*>(&other);
//    std::copy(src, src+other._sizeof(), reinterpret_cast<char*>(this));
///    return *this;
//  }
//  /** Mask of requested BLD */
//  uint64_t bldMask() const { return _bldMask; }
//  /** Number of source definitions */
//  uint32_t numSources() const { return _numSources; }
//  /** Source configuration objects **/
//
//private:
//  uint64_t	_bldMask;	/**< Mask of requested BLD */
//  uint32_t	_numSources;	/**< Number of source definitions */
//  //Partition::Source	_sources[this->numSources()];
//};

class ConfigV2 {
public:
  enum { TypeId = TypeId_xtc1::Id_PartitionConfig /**< XTC type ID value (from Pds::TypeId class) */ };
  enum { Version = 2 /**< XTC type version number */ };
  ConfigV2(uint32_t arg__numWords, uint32_t arg__numSources, const uint32_t* arg__bldMask, const Source* arg__sources);
  ConfigV2() {}
  ConfigV2(const ConfigV2& other) {
    const char* src = reinterpret_cast<const char*>(&other);
    std::copy(src, src+other._sizeof(), reinterpret_cast<char*>(this));
  }
  ConfigV2& operator=(const ConfigV2& other) {
    const char* src = reinterpret_cast<const char*>(&other);
    std::copy(src, src+other._sizeof(), reinterpret_cast<char*>(this));
    return *this;
  }
  /** Number of words for the bit mask */
  uint32_t numWords() const { return _numWords; }
  /** Number of source definitions */
  uint32_t numSources() const { return _numSources; }
  /** Returns the total number of bits in the mask */
  uint32_t numBldMaskBits() const;
  /** Returns non-zero if all bits in the mask are unset, zero otherwise. */
  uint32_t bldMaskIsZero() const;
  /** Returns non-zero if any bits in the mask are set, zero otherwise. */
  uint32_t bldMaskIsNotZero() const;
  /** Returns non-zero if the bit cooresponding to iBit in the word is set, zero otherwise. */
  uint32_t bldMaskHasBitSet(uint32_t iBit) const;
  /** Returns non-zero if the bit cooresponding to iBit in the word is unset, zero otherwise. */
  uint32_t bldMaskHasBitClear(uint32_t iBit) const;
  uint32_t _sizeof() const { return (((((8+(4*(this->numWords())))+(Source::_sizeof()*(this->numSources())))+4)-1)/4)*4; }
private:
  uint32_t	_numWords;	/**< Number of words for the bit mask */
  uint32_t	_numSources;	/**< Number of source definitions */
  //uint32_t	_bldMask[this->numWords()];
  //Partition::Source	_sources[this->numSources()];

public:
template <typename T>
std::vector<Source> sources(const std::shared_ptr<T>& owner) const
{
    ptrdiff_t offset = 8 + 4 * this->numWords();
    const Source* data = reinterpret_cast<const Source*>(
        reinterpret_cast<const char*>(this) + offset
    );

    size_t n = this->numSources();
    return std::vector<Source>(data, data + n);
}
};

} // Namespace XtcData

#endif
