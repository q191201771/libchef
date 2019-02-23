/**
 * @license  this file is a part of libchef. more info see https://github.com/q191201771/libchef
 * @tag      v1.10.17
 * @file     chef_be_le_op.hpp
 * @deps     nope
 * @platform linux | macos | xxx
 *
 * @author
 *   chef <191201771@qq.com>
 *     - initial release xxxx-xx-xx
 *
 * @brief    读写大小端二进制流
 *
 */

#ifndef _CHEF_BASE_BE_LE_OP_HPP_
#define _CHEF_BASE_BE_LE_OP_HPP_
#pragma once

#include <cinttypes>
#include <cstddef>

namespace chef {

class be_le_op {
  public:
    static bool check_local_if_le() { int32_t val=1; int8_t *p = reinterpret_cast<int8_t *>(&val); return *p == 1; }

    static uint8_t  read_be_ui8(const uint8_t *p)  { return *p; }
    static uint16_t read_be_ui16(const uint8_t *p) { uint16_t val=0; val |= (p[0] << 8); val |= p[1]; return val; }
    static uint32_t read_be_ui24(const uint8_t *p) { uint32_t val=0; val |= (p[0] << 16); val |= (p[1] << 8); val |= p[2]; return val; }
    static uint32_t read_be_ui32(const uint8_t *p) { uint32_t val=0; val |= (p[0] << 24); val |= (p[1] << 16); val |= (p[2] << 8); val |= p[3]; return val; }

    static uint32_t read_le_ui32(const uint8_t *p) { uint32_t val=0; val |= (p[3] << 24); val |= (p[2] << 16); val |= (p[1] << 8); val |= p[0]; return val; }

    static uint8_t *write_be_ui8(uint8_t *p, uint8_t val)   { p[0] = val; return p+1; }
    static uint8_t *write_be_ui16(uint8_t *p, uint16_t val) { p[0] = val >> 8; p[1] = val & 0xff; return p+2; }
    static uint8_t *write_be_ui24(uint8_t *p, uint32_t val) { p[0] = val >> 16; p[1] = val >> 8; p[2] = val & 0xff; return p+3; }
    static uint8_t *write_be_ui32(uint8_t *p, uint32_t val) { p[0] = val >> 24; p[1] = val >> 16; p[2] = val >> 8; p[3] = val & 0xff; return p+4; }

    static uint8_t *write_le_ui32(uint8_t *p, uint32_t val) { p[0] = val & 0xff; p[1] = val >> 8; p[2] = val >> 16; p[3] = val >> 24; return p+4; }
};

}

#endif
