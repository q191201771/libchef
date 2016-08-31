/**
 * @file   stuff.h/stuff.cc
 * @deps   nope
 *
 * @author
 *   chef <191201771@qq.com>
 *     -initial release 2016-08-31
 *
 * @brief  暂时没有分类的功能函数放这里，后续再整理～
 *
 */

#ifndef _CHEF_BASE_STUFF_H_
#define _CHEF_BASE_STUFF_H_

#include <string>
#include <vector>
#include <stdint.h>

namespace chef {

  class stuff {
    public:
      /**
       * returns the number of processors which are currently online (i.e., available).
       *
       */
      static int get_cpu_num();

      /// @return e.g. /home/chef/starry-night/build/base_test/stuff_test
      static std::string get_exe_filepath();

      /// @return e.g. /home/chef/starry-night/build/base_test/
      static std::string get_exe_path();

      /// @return e.g. stuff_test
      static std::string get_exe_name();

      /**
       * 将以字节为单位的数字转换为带大小单位的可读性字符串
       *
       * @param n bytes
       *
       * @return
       *   e.g.
       *   readable_bytes(768) = "768.0B"
       *   readable_bytes(10000) = "9.8K"
       *   readable_bytes(100001221) = "95.4M"
       *
       */
      static std::string readable_bytes(uint64_t n);

      /**
       * 获取域名对应的ip
       *
       * @param domain 域名
       *
       * @return ip
       *   成功例子：
       *     "www.baidu.com"
       *     "localhost"
       *     "58.96.168.38"
       *   失败例子：
       *     "not exist"
       *     "http://www.baidu.com"
       *
       */
      static std::string get_host_by_name(const std::string &domain);

      //static int getpid();
      //static int getpagesize();

  };

} // namespace chef

#endif // _CHEF_BASE_STUFF_H_
