/**
 * @license  this file is a part of libchef. more info see https://github.com/q191201771/libchef
 * @tag      v1.10.17
 * @file     chef_crypto_hmac_sha256.hpp
 * @deps     chef_crypto_sha256_op.hpp
 * @platform linux | macos | xxx
 *
 * @author
 *   chef <191201771@qq.com>
 *     -initial release xxxx-xx-xx
 *
 * @brief
 *   - hmac sha256加密
 *   - @NOTICE 实现部分拷贝自https://github.com/lyokato/cpp-cryptlite/blob/master/include/cryptlite/hmac.h
 *
     ```
     // 示例
     uint8_t key[] = {
  	  0x0b, 0x0b, 0x0b, 0x0b, 0x0b, 0x0b, 0x0b, 0x0b,
  	  0x0b, 0x0b, 0x0b, 0x0b, 0x0b, 0x0b, 0x0b, 0x0b,
  	  0x0b, 0x0b, 0x0b, 0x0b
     };
     std::size_t key_len = 20;
     const char *buf = "Hi There";
     std::size_t len = 8;

     chef::crypto_hmac_sha256 ctx(key, 20);
     ctx.update((const uint8_t *)buf, 8);
     uint8_t digest[32];
     ctx.final(digest);
     uint8_t result[] = {
       0xb0, 0x34, 0x4c, 0x61, 0xd8, 0xdb, 0x38, 0x53,
       0x5c, 0xa8, 0xaf, 0xce, 0xaf, 0x0b, 0xf1, 0x2b,
       0x88, 0x1d, 0xc2, 0x00, 0xc9, 0x83, 0x3d, 0xa7,
       0x26, 0xe9, 0x37, 0x6c, 0x2e, 0x32, 0xcf, 0xf7
     };
     assert(memcmp(digest, result, 32) == 0);

     ```
 *
 */

#ifndef _CHEF_BASE_CRYPTO_HMAC_SHA256_HPP_
#define _CHEF_BASE_CRYPTO_HMAC_SHA256_HPP_
#pragma once

#include "chef_crypto_sha256_op.hpp"
#include <string>

namespace chef {

  namespace cryptlite__ {
    class sha256;
    template <typename T> class hmac;
  }

  class crypto_hmac_sha256 {
    public:
      crypto_hmac_sha256(const uint8_t *key, std::size_t key_len);
      ~crypto_hmac_sha256();
      void update(const uint8_t *buf, std::size_t len);
      void final(uint8_t dst[32] /* out */);

    private:
      crypto_hmac_sha256(const crypto_hmac_sha256 &);
      crypto_hmac_sha256 &operator=(const crypto_hmac_sha256 &);

    private:
      cryptlite__::hmac<cryptlite__::sha256> *ctx_;

  }; // class crypto_hmac_sha256

} // namespace chef





///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// @NOTICE 该分隔线以上部分为该模块的接口，分割线以下部分为对应的实现





/*
The MIT License

Copyright (c) 2011 lyo.kato@gmail.com

Permission is hereby granted, free of charge, to any person obtaining a copy
of this software and associated documentation files (the "Software"), to deal
in the Software without restriction, including without limitation the rights
to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
copies of the Software, and to permit persons to whom the Software is
furnished to do so, subject to the following conditions:

The above copyright notice and this permission notice shall be included in
all copies or substantial portions of the Software.

THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN
THE SOFTWARE.
*/

//#ifndef _CRYPTLITE_HMAC_H_
//#define _CRYPTLITE_HMAC_H_

#include <string>
#include <sstream>
#include <cstring>
#include <cassert>
#include <iomanip>
//#include <boost/cstdint.hpp>

namespace chef {

namespace cryptlite__ {

template <typename T>
class hmac {

public:

    static const unsigned int BLOCK_SIZE = T::BLOCK_SIZE;
    static const unsigned int HASH_SIZE  = T::HASH_SIZE;

    static void calc(
            const char* text, int text_len,
            const char* key,  int key_len,
            uint8_t digest[HASH_SIZE]) {
        assert(digest);
        calc(reinterpret_cast<const uint8_t*>(text), text_len,
             reinterpret_cast<const uint8_t*>(key), key_len, digest);
    }

    static void calc(
            const uint8_t* text, int text_len,
            const uint8_t* key,  int key_len,
            uint8_t digest[HASH_SIZE]) {
        assert(digest);
        hmac<T> ctx(key, key_len);
        ctx.input(text, text_len);
        ctx.result(digest);
    }

    inline static void calc(
            const std::string& text,
            const std::string& key,
            uint8_t digest[HASH_SIZE]) {
        assert(digest);
        calc(reinterpret_cast<const char*>(text.c_str()), text.size(),
             reinterpret_cast<const char*>(key.c_str()), key.size(), digest);
    }

    inline static std::string calc_hex(
            const std::string& text,
            const std::string& key ) {
        return calc_hex(reinterpret_cast<const uint8_t*>(text.c_str()), text.size(),
                reinterpret_cast<const uint8_t*>(key.c_str()), key.size());
    }

    static std::string calc_hex(
            const uint8_t* text, int text_len,
            const uint8_t* key,  int key_len ) {
        int i;
        uint8_t digest[HASH_SIZE];
        assert(key);
        assert(text);
        std::ostringstream oss;
        oss << std::hex << std::setfill('0');
        hmac<T> ctx(key, key_len);
        ctx.input(text, text_len);
        ctx.result(digest);
        for (i = 0; i < HASH_SIZE; i++)
            oss << std::setw(2) << (digest[i] & 0xff);
        oss << std::dec;
        return oss.str();
    }

    hmac(const uint8_t* key, int key_len) : hasher_(T()) {
        assert(key);
        reset(key, key_len);
    }

    hmac(const std::string& key) : hasher_(T()) {
        reset(reinterpret_cast<const uint8_t*>(key.c_str()), key.size());
    }

    ~hmac() { }

    inline void reset(const std::string& key) {
        reset(reinterpret_cast<const uint8_t*>(key.c_str()), key.size());
    }

    void reset(const uint8_t* key, int key_len) {

        int i;
        uint8_t k_ipad[BLOCK_SIZE];
        uint8_t tempkey[HASH_SIZE];

        assert(key);

        if (key_len > static_cast<int>(BLOCK_SIZE)) {
            T sha;
            sha.input(key, key_len);
            sha.result(tempkey);
            key = tempkey;
            key_len = HASH_SIZE;
        }

        for (i=0; i < key_len; i++) {
            k_ipad[i]  = key[i] ^ 0x36;
            k_opad_[i] = key[i] ^ 0x5c;
        }

        for (; i < static_cast<int>(BLOCK_SIZE); i++) {
            k_ipad[i]  = 0x36;
            k_opad_[i] = 0x5c;
        }

        hasher_.reset();
        hasher_.input(k_ipad, static_cast<int>(BLOCK_SIZE));
    }

    inline void input(const std::string& text) {
        input(reinterpret_cast<const uint8_t*>(text.c_str()), text.size());
    }

    void input(const uint8_t* text, int text_len) {
        assert(text);
        hasher_.input(text, text_len);
    }

    void final_bits(const uint8_t bits, unsigned int bitcount) {
        hasher_.final_bits(bits, bitcount);
    }

    void result(uint8_t digest[HASH_SIZE]) {
        assert(digest);
        hasher_.result(digest);
        hasher_.reset();
        hasher_.input(k_opad_, BLOCK_SIZE);
        hasher_.input(digest, HASH_SIZE);
        hasher_.result(digest);
    }

private:
    uint8_t k_opad_[BLOCK_SIZE];
    T hasher_;
}; // end of class

}  // end of namespace

inline crypto_hmac_sha256::crypto_hmac_sha256(const uint8_t *key, std::size_t key_len) {
  ctx_ = new cryptlite__::hmac<cryptlite__::sha256>(key, key_len);
}

inline crypto_hmac_sha256::~crypto_hmac_sha256() {
  delete ctx_;
}

inline void crypto_hmac_sha256::update(const uint8_t *buf, std::size_t len) {
  ctx_->input(buf, len);
}

inline void crypto_hmac_sha256::final(uint8_t dst[32] /* out */) {
  ctx_->result(dst);
}

} // namespace chef

#endif
