/**
 * @file     chef_defer.hpp
 * @deps     nope
 * @platform linux/macos/xxx
 *
 * @author
 *   chef <191201771@qq.com>
 *     - initial release xxxx-xx-xx
 *
 * @brief  类似golang defer，支持c goto清理等场景
 *
 */

#ifndef _CHEF_BASE_DEFER_HPP_
#define _CHEF_BASE_DEFER_HPP_
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

#endif
