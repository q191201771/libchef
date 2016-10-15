/**
 * @file   logo.h/logo.cc
 * @deps   nope
 *
 * @author
 *   chef <191201771@qq.com>
 *     -initial release xxxx-xx-xx
 *
 * @brief  读取指定文件中的内容，打印到stdout
 *
 */

#ifndef _CHEF_BASE_LOGO_H_
#define _CHEF_BASE_LOGO_H_

#include <string>

namespace chef {

  class logo {
    public:
      logo(const std::string &filename);
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
