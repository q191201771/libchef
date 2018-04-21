/**
 * @file     chef_defer.hpp
 * @deps     nope
 * @platform linux/macos/xxx
 *
 * @author
 *   chef <191201771@qq.com>
 *     -initial release xxxx-xx-xx
 *
 * @brief nope
 *
 */

#pragma once

#include <functional>

namespace chef {

class defer {
  public:
    typedef std::function<void()> task;

  public:
    defer(const task &t=nullptr) : cancel_(false), t_(t) {}

    ~defer() {
      if (!cancel_ && t_) {
        t_();
      }
    }

    void cancel() { cancel_ = true; }

  private:
    bool cancel_;
    task t_;
}; // class defer

}; // namespace chef

// #include <string>
// int main() {
//   int a = 10;
//   std::string str = "aaa";
//   std::string f = __FUNCTION__;
//   chef::defer x([&a, &str, &f]() {
//     printf("here. %d %s %s\n", a, str.c_str(), f.c_str());
//     //str = "ccc";
//   });
//
//   str = "bbb";
//   printf("%d %s\n", a, str.c_str());
//
//   if (1) {
//     return 0;
//   }
//
//   if (0) {
//     x.cancel();
//     return 0;
//   }
//
//   return 0;
// }
