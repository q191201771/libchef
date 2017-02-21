/**
 * @file   logo.h/logo.cc
 * @deps   chef_noncopyable.hpp
 *
 * @author
 *   chef <191201771@qq.com>
 *     -initial release xxxx-xx-xx
 *
 * @brief
 *   读取指定文件中的内容，打印到stdout
 *   chef.logo中的图抄自 https://github.com/channelcat/sanic/blob/master/sanic/config.py
 *
 */

#ifndef _CHEF_BASE_LOGO_H_
#define _CHEF_BASE_LOGO_H_

#include <string>
#include "chef_noncopyable.hpp"

namespace chef {

  class logo : public chef::noncopyable {
    public:
      explicit logo(const std::string &filename);
      ~logo();

      int init();

      void flush_to_stdout();

      std::string get_content() const { return content_; }

    private:
      std::string filename_;
      std::string content_;
  };

} // namespace chef

#define FLUSH_CHEF_LOGO \
  do { \
    chef::logo l("chef.logo"); \
    l.init(); \
    l.flush_to_stdout(); \
  }  while(0);

#endif // _CHEF_BASE_LOGO_H_
