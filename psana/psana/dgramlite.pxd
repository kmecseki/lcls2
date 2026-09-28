from libc.stdint cimport uint64_t, uint32_t, uint16_t, uint8_t
from libc.string cimport memcpy

cdef struct Src:
    uint32_t src

cdef struct Damage:
    uint16_t damage

cdef struct TypeId:
    uint16_t value

cdef struct Xtc:
    Src src
    Damage damage
    TypeId contains
    uint32_t extent

cdef struct Sequence:
    uint32_t low
    uint32_t high

cdef struct Dgram:
    Sequence seq
    uint32_t env
    Xtc xtc

cdef struct Src_xtc1:
    uint32_t log
    uint32_t phy

cdef struct Damage_xtc1:
    uint32_t damage

cdef struct TypeId_xtc1:
    uint32_t value

cdef struct Xtc1:
    Damage_xtc1 damage
    Src_xtc1 src
    TypeId_xtc1 contains
    uint32_t extent

cdef struct Dgram_xtc1:
    Sequence clock
    Sequence seq
    uint32_t env
    Xtc1 xtc
