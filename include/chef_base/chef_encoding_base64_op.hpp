/**
 * @license  this file is part of starry-night library. more info see https://github.com/q191201771/starry-night
 * @tag      v1.7.17
 * @file     chef_encoding_base64_op.hpp
 * @deps     nope
 * @platform linux | macos | xxx
 *
 * @author
 *   chef <191201771@qq.com>
 *     - initial release 2016-08-31
 *
 * @brief
 *   - base64编码、解码
 *   - @NOTICE copy from https://github.com/zaphoyd/websocketpp/blob/master/websocketpp/base64/base64.hpp
 *
     ```
     std::string origin = "foob";
     std::string res = chef::encoding_base64_op(origin);
     //assert(res == "Zm9vYg==");
     origin = chef::encoding_base64_op::decode(res);
     //assert(origin == "foob");
     ```
 *
 */

#ifndef _CHEF_BASE_BASE64_HPP_
#define _CHEF_BASE_BASE64_HPP_
#pragma once

#include <string>

namespace chef {

  class encoding_base64_op {
    public:
      static std::string encode(const std::string &s);
      static std::string encode(const char *data, std::size_t len);


      static std::string decode(const std::string &s);
      static std::string decode(const char *data, std::size_t len);

    private:
      encoding_base64_op();
      encoding_base64_op(const encoding_base64_op &);
      encoding_base64_op &operator=(const encoding_base64_op &);

  }; // class encoding_base64_op

} // namespace chef





///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// @NOTICE 该分隔线以上部分为该模块的接口，分割线以下部分为对应的实现





namespace chef {

  namespace inner {
    static inline std::string base64_encode(unsigned char const * input, size_t len);
    static inline std::string base64_decode(unsigned char const * input, size_t len);
  }

  inline std::string encoding_base64_op::encode(const char *data, std::size_t len) {
    return inner::base64_encode((unsigned char const *)data, len);
  }

  inline std::string encoding_base64_op::encode(const std::string &s) {
    return encode(s.c_str(), s.length());
  }

  inline std::string encoding_base64_op::decode(const char *data, std::size_t len) {
    return inner::base64_decode((unsigned char const *)data, len);
  }

  inline std::string encoding_base64_op::decode(const std::string &s) {
    return decode(s.c_str(), s.length());
  }
} /// namespace chef



/// ----- 下面的代码是拷贝过来的 ----------------------------------------------

/*
    ******
    base64.hpp is a repackaging of the base64.cpp and base64.h files into a
    single header suitable for use as a header only library. This conversion was
    done by Peter Thorson (webmaster@zaphoyd.com) in 2012. All modifications to
    the code are redistributed under the same license as the original, which is
    listed below.
    ******

   base64.cpp and base64.h

   Copyright (C) 2004-2008 René Nyffenegger

   This source code is provided 'as-is', without any express or implied
   warranty. In no event will the author be held liable for any damages
   arising from the use of this software.

   Permission is granted to anyone to use this software for any purpose,
   including commercial applications, and to alter it and redistribute it
   freely, subject to the following restrictions:

   1. The origin of this source code must not be misrepresented; you must not
      claim that you wrote the original source code. If you use this source code
      in a product, an acknowledgment in the product documentation would be
      appreciated but is not required.

   2. Altered source versions must be plainly marked as such, and must not be
      misrepresented as being the original source code.

   3. This notice may not be removed or altered from any source distribution.

   René Nyffenegger rene.nyffenegger@adp-gmbh.ch

*/

namespace chef {
namespace inner {

static const std::string base64_chars =
             "ABCDEFGHIJKLMNOPQRSTUVWXYZ"
             "abcdefghijklmnopqrstuvwxyz"
             "0123456789+/";

/// Test whether a character is a valid base64 character
/**
 * @param c The character to test
 * @return true if c is a valid base64 character
 */
static inline bool is_base64(unsigned char c) {
    return (c == 43 || // +
           (c >= 47 && c <= 57) || // /-9
           (c >= 65 && c <= 90) || // A-Z
           (c >= 97 && c <= 122)); // a-z
}

/// Encode a char buffer into a base64 string
/**
 * @param input The input data
 * @param len The length of input in bytes
 * @return A base64 encoded string representing input
 */
static inline std::string base64_encode(unsigned char const * input, size_t len) {
    std::string ret;
    int i = 0;
    int j = 0;
    unsigned char char_array_3[3];
    unsigned char char_array_4[4];

    while (len--) {
        char_array_3[i++] = *(input++);
        if (i == 3) {
            char_array_4[0] = (char_array_3[0] & 0xfc) >> 2;
            char_array_4[1] = ((char_array_3[0] & 0x03) << 4) +
                              ((char_array_3[1] & 0xf0) >> 4);
            char_array_4[2] = ((char_array_3[1] & 0x0f) << 2) +
                              ((char_array_3[2] & 0xc0) >> 6);
            char_array_4[3] = char_array_3[2] & 0x3f;

            for(i = 0; (i <4) ; i++) {
                ret += base64_chars[char_array_4[i]];
            }
            i = 0;
        }
    }

    if (i) {
        for(j = i; j < 3; j++) {
            char_array_3[j] = '\0';
        }

        char_array_4[0] = (char_array_3[0] & 0xfc) >> 2;
        char_array_4[1] = ((char_array_3[0] & 0x03) << 4) +
                          ((char_array_3[1] & 0xf0) >> 4);
        char_array_4[2] = ((char_array_3[1] & 0x0f) << 2) +
                          ((char_array_3[2] & 0xc0) >> 6);
        char_array_4[3] = char_array_3[2] & 0x3f;

        for (j = 0; (j < i + 1); j++) {
            ret += base64_chars[char_array_4[j]];
        }

        while((i++ < 3)) {
            ret += '=';
        }
    }

    return ret;
}

/// Encode a string into a base64 string
/**
 * @param input The input data
 * @return A base64 encoded string representing input
 */
static inline std::string base64_encode(std::string const & input) {
    return base64_encode(
        reinterpret_cast<const unsigned char *>(input.data()),
        input.size()
    );
}

/// Decode a base64 encoded string into a string of raw bytes
/**
 * @param input The base64 encoded input data
 * @return A string representing the decoded raw bytes
 */
//static inline std::string base64_decode(std::string const & input) {
static inline std::string base64_decode(unsigned char const * input, size_t len) {
    //size_t in_len = input.size();
    size_t in_len = len;
    int i = 0;
    int j = 0;
    int in_ = 0;
    unsigned char char_array_4[4], char_array_3[3];
    std::string ret;

    while (in_len-- && ( input[in_] != '=') && is_base64(input[in_])) {
        char_array_4[i++] = input[in_]; in_++;
        if (i ==4) {
            for (i = 0; i <4; i++) {
                char_array_4[i] = static_cast<unsigned char>(base64_chars.find(char_array_4[i]));
            }

            char_array_3[0] = (char_array_4[0] << 2) + ((char_array_4[1] & 0x30) >> 4);
            char_array_3[1] = ((char_array_4[1] & 0xf) << 4) + ((char_array_4[2] & 0x3c) >> 2);
            char_array_3[2] = ((char_array_4[2] & 0x3) << 6) + char_array_4[3];

            for (i = 0; (i < 3); i++) {
                ret += char_array_3[i];
            }
            i = 0;
        }
    }

    if (i) {
        for (j = i; j <4; j++)
            char_array_4[j] = 0;

        for (j = 0; j <4; j++)
            char_array_4[j] = static_cast<unsigned char>(base64_chars.find(char_array_4[j]));

        char_array_3[0] = (char_array_4[0] << 2) + ((char_array_4[1] & 0x30) >> 4);
        char_array_3[1] = ((char_array_4[1] & 0xf) << 4) + ((char_array_4[2] & 0x3c) >> 2);
        char_array_3[2] = ((char_array_4[2] & 0x3) << 6) + char_array_4[3];

        for (j = 0; (j < i - 1); j++) {
            ret += static_cast<std::string::value_type>(char_array_3[j]);
        }
    }

    return ret;
}

} /// namespace inner
} /// namespace chef

#endif /// _CHEF_BASE_BASE64_HPP_
