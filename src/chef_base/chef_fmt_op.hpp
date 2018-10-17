/**
 * @tag      v1.4.9
 * @file     chef_fmt_op.hpp
 * @deps     c++11
 * @platform linux | macos | xxx
 *
 * @author
 *   chef <191201771@qq.com>
 *     - initial release xxxx-xx-xx
 *
 * @brief    方便的生成格式化字符串，类似于sprintf，格式符由`%d` `%s`等等简化成统一的`{}`
 *
     ```
     // 1. 基本用法
     std::string res = chef::fmt_op::format("I am {}, and {} year old now!", "chef", 18);
     //assert(res == "I am chef, and 18 year old now!");

     // 2. 支持自定义类型，只需重载格式化运算符即可
     struct student {
       std::string name;
       int age;

       friend std::ostream &operator<<(std::ostream &os, const student &s) {
         return os << "{" << s.name << "," << s.age << "}";
       }
     };

     student s; s.name = "yoko"; s.age = 1;
     res = chef::fmt_op::format("student info:{}", s);
     //assert(res == "student info:{yoko,1}");
     ```
 *
 */

#ifndef _CHEF_BASE_FMT_OP_HPP_
#define _CHEF_BASE_FMT_OP_HPP_
#pragma once

#include <string>
#include <sstream>

namespace chef {

  class fmt_op {
  public:
    template<typename ...Args>
    static std::string format(const std::string &fmt, Args... args) {
      return format(fmt.c_str(), args...);
    }

    template<typename ...Args>
    static std::string format(const char *fmt, Args... args) {
      std::string res(fmt);

      if (sizeof...(args) == 0) { return res; }

      format(res, args...);

      return res;
    }

  private:
    template<typename T, typename... Args>
    static void format(std::string &fmt, const T &val, const Args&... args) {
      format(fmt, val);
      format(fmt, args...);
    }

    template<typename T>
    static void format(std::string &fmt, const T &val) {
      std::ostringstream oss;
      oss << val;

      std::size_t pos = fmt.find("{}");
      if (pos != std::string::npos) { fmt.replace(pos, 2, oss.str()); }
    }
  };

} // namespace chef

#endif // _CHEF_BASE_FMT_OP_HPP_

