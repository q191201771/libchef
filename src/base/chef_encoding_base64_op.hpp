/**
 * @file   chef_encoding_base64_op.hpp[_impl]
 * @deps   nope
 * @platform linux | macos | xxx
 *
 * @author
 *   chef <191201771@qq.com>
 *     -initial release 2016-08-31
 *
 * @brief
 *   base64编码、解码
 *
 *   copy from https://github.com/zaphoyd/websocketpp/blob/master/websocketpp/base64/base64.hpp
 *
 */

#ifndef _CHEF_BASE_BASE64_H_
#define _CHEF_BASE_BASE64_H_

#include <string>

namespace chef {

  class encoding_base64_op {
    public:
      static std::string encode(const std::string &s);

      static std::string decode(const std::string &s);
  };

} /// namespace chef

#include "chef_encoding_base64_op_impl.hpp"

#endif /// _CHEF_BASE_BASE64_H_
