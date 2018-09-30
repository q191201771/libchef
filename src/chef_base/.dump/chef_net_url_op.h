/**
 * @file   chef_net_url_op.h/chef_net_url_op.cc
 * @deps   nope
 *
 * @author
 *   chef <191201771@qq.com>
 *     -created 2017-11-23 19:55:12
 *     -initial release xxxx-xx-xx
 *
 * @brief
 *   xxx
 *
 */

#pragma once

#include <string>

namespace chef {

  static const std::string URL_QUERY_ESCAPING_CHARACTERS = "$&+,/:;=?@";

  class net_url_op {
    public:
      // 编码使得可以安全的放入URL Query中
      // @NOTICE 空格会被编码成加号，即 '+'
      static std::string query_escape(const std::string &s);

      static std::string query_unescape(const std::string &s);

  }; /// class net_url_op

} /// namespace chef

