// Copyright 2022, Yoko.  All rights reserved.
//
// Author: Yoko (191201771@qq.com)

// 保证某个代码段只被执行一次

#ifndef CHEF_BASE_SYNC_ONCE_HPP_
#define CHEF_BASE_SYNC_ONCE_HPP_
#pragma once

#include <sstream>
#include <string>

namespace chef {

  class sync_once {
  public:
    void run(std::function<void()> f);

  private:
    std::atomic<bool> done_;
    std::mutex        m_;
  };

  // -------------------------------------------------------------------------------------------------------------------

  inline void sync_once::run(std::function<void()> f) {
    if (done_ == 0) {
      m_.lock();
      if (done_ == 0) {
        f();
        done_ = 1;
      }
      m_.unlock();
    }
  }

}

#endif