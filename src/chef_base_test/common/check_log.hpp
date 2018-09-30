/**
 * @file   check_log.hpp
 * @deps   nope
 *
 * @author
 *   chef <191201771@qq.com>
 *     -initial release xxxx-xx-xx
 *
 * @brief
 *   构造、析构时分别打印一行带颜色的字符串，用于多个程序串行跑unit test时显示。
 *
 */

#ifndef _BASE_TEST_COMMON_CHECK_LOG_HPP_
#define _BASE_TEST_COMMON_CHECK_LOG_HPP_

#include <string>
#include <stdio.h>
#include <iostream>

namespace chef {

  class check_log {
    public:
      check_log(const std::string &name) : name_(name) {
        printf("\033[22;31mCheck %s.\n\033[0m", name.c_str());
      }
      ~check_log() {
        printf("\033[22;36mCheck %s done.\n\033[0m", name_.c_str());
      }

    private:
      std::string name_;
  };

} // namespace chef

#define ENTER_TEST chef::check_log _bASe_TEST_COMMON_CHECK_LOG_HPP_ccl(__FILE__);

#endif // _BASE_TEST_COMMON_CHECK_LOG_HPP_
