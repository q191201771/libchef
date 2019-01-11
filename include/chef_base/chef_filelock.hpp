/**
 * @license  this file is a part of libchef. more info see https://github.com/q191201771/libchef
 * @tag      v1.9.17
 * @file     chef_filelock.hpp
 * @deps     nope
 * @platform linux | macos | xxx
 *
 * @author
 *   chef <191201771@qq.com>
 *     -initial release xxxx-xx-xx
 *
 * @brief
 *   文件锁，可用于多进程间（无亲缘关系的进程间也可以）的互斥锁，并且是进程崩溃安全的（即进程退出时会自动释放持有的文件锁）。
 *
 *   补充说明：
 *   * 文件锁也可以在多线程环境使用（多个线程使用多个 chef::filelock 对象绑定同一个 filename），但是性能比pthread mutex差。
 *   * 对于单个 chef::filelock 对象，获取锁动作是可重入的（即lock成功后不unlock可以再次lock），且和递归锁不同，
 *     释放次数不需要和获取次数匹配（比如lock两次再unlock一次即可完全释放锁）。但从程序语意上尽量保持简单与正确的出发点来说，
 *     我还是建议业务方不要重入锁，且锁获取和释放保持一一对应。
 *
 */

#ifndef _CHEF_BASE_FILELOCK_HPP_
#define _CHEF_BASE_FILELOCK_HPP_
#pragma once

#include <string>

namespace chef {

  class filelock {
    public:
      /// 使用锁之前有且仅有调用一次
      bool attach(const char *filename);

      /// 获取到锁则直接返回，获取不到则阻塞等待
      bool lock();

      /// 非阻塞式尝试获取锁，成功获取到锁返回true，获取失败返回false
      bool try_lock();

      /// 释放锁
      bool unlock();

    public:
      filelock() : fd_(-1) {}
      ~filelock() { detach_if_needed(); }

    private:
      void detach_if_needed();

    private:
      std::string filename_;
      int         fd_;

    private:
      filelock(const filelock &);
      filelock &operator=(const filelock &);

  }; // class filelock

} // namespace chef





///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// @NOTICE 该分隔线以上部分为该模块的接口，分割线以下部分为对应的实现





#include <assert.h>
#include <errno.h>
#include <fcntl.h>
#include <stdio.h>
#include <string.h>
#include <unistd.h>

namespace chef {

inline bool filelock::attach(const char *filename) {
  if (!filename || fd_ != -1) { assert(0); return false; }

  filename_ = filename;
  fd_ = open(filename, O_RDWR | O_CREAT, 0644);
  if (fd_ == -1) {
    fprintf(stderr, "%s:%d open %s failed,errno:%d(%s).", __FILE__, __LINE__, filename, errno, strerror(errno));
    return false;
  }

  return true;
}

inline void filelock::detach_if_needed() {
  if (fd_ == -1) { return; }

  //if (unlink(filename_.c_str()) == -1) {
  //  fprintf(stderr, "%s:%d open %s failed,errno:%d(%s).", __FILE__, __LINE__, filename_.c_str(), errno, strerror(errno));
  //}

  if(close(fd_) == -1) {
    fprintf(stderr, "%s:%d close %s failed,errno:%d(%s).", __FILE__, __LINE__, filename_.c_str(), errno, strerror(errno));
  }
  fd_ = -1;
}

inline bool filelock::lock() {
  assert(fd_ != -1);

  struct flock fl;
  memset(&fl, 0x0, sizeof(struct flock));
  fl.l_type = F_WRLCK;
  fl.l_whence = SEEK_SET;
  if (fcntl(fd_, F_SETLKW, &fl) == -1) {
    fprintf(stderr, "%s:%d fcntl %s failed,errno:%d(%s).", __FILE__, __LINE__, filename_.c_str(), errno, strerror(errno));
    return false;
  }

  return true;
}

inline bool filelock::try_lock() {
  assert(fd_ != -1);

  struct flock fl;
  memset(&fl, 0x0, sizeof(struct flock));
  fl.l_type = F_WRLCK;
  fl.l_whence = SEEK_SET;
  if (fcntl(fd_, F_SETLK, &fl) == -1) {
    if (errno != EAGAIN) {
      fprintf(stderr, "%s:%d fcntl %s failed,errno:%d(%s).", __FILE__, __LINE__, filename_.c_str(), errno, strerror(errno));
    }
    return false;
  }

  return true;

}

inline bool filelock::unlock() {
  assert(fd_ != -1);

  struct flock fl;
  memset(&fl, 0x0, sizeof(struct flock));
  fl.l_type = F_UNLCK;
  fl.l_whence = SEEK_SET;
  if (fcntl(fd_, F_SETLK, &fl) == -1) {
    fprintf(stderr, "%s:%d fcntl %s failed,errno:%d(%s).", __FILE__, __LINE__, filename_.c_str(), errno, strerror(errno));
    return false;
  }

  return true;
}

}


#endif
