/**
 * @file   chef_crypto_md5_op.hpp[_impl]
 * @deps   nope
 * @platform linux | macos | xxx
 *
 * @author
 *   chef <191201771@qq.com>
 *     - initial release 2016-08-31
 *
 * @brief
 *   - md5加密
 *   - copy from https://github.com/zaphoyd/websocketpp/blob/master/websocketpp/common/md5.hpp
 *
 */

#ifndef _CHEF_BASE_CRYPTO_MD5_OP_HPP_
#define _CHEF_BASE_CRYPTO_MD5_OP_HPP_
#pragma once

#include <string>

namespace chef {

  class crypto_md5_op {
    public:
      static std::string sum(const std::string &s);
  };

} // namespace chef

#include "chef_crypto_md5_op_impl.hpp"

#endif // _CHEF_BASE_CRYPTO_MD5_OP_HPP_
