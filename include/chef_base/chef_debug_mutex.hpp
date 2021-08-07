/**
 * @license  this file is a part of libchef. more info see https://github.com/q191201771/libchef
 * @tag      v1.10.17
 * @file     chef_debug_mutex.hpp
 * @deps     nope
 * @platform linux | macos | xxx
 *
 * @author
 *   chef <191201771@qq.com>
 *     - initial release xxxx-xx-xx
 *
 * @brief    定位互斥锁的问题
 *
 */
#ifndef _CHEF_BASE_DEBUG_MUTEX_HPP_
#define _CHEF_BASE_DEBUG_MUTEX_HPP_

#include "chef_env.hpp"
#include "chef_strings_op.hpp"
#include "chef_stuff_op.hpp"

#include <time.h>
#include <stdio.h>
#include <unistd.h>
#include <sys/time.h>
#include <execinfo.h>
#include <map>
#include <vector>
#include <thread>
#include <sstream>

// 使用方式见 libchef/test/chef_base_test/chef_debug_mutex_test.cc
// link -rdynamic
// addr2line -e a.out

namespace chef {

namespace inner {

inline int dm_gettid() {
  return chef::stuff_op::gettid();
}

inline std::uint64_t dm_tick_msec() {
  return chef::stuff_op::tick_msec();
}

inline std::string dm_join(const std::vector<std::string> &ss, const std::string &sep) {
  return chef::strings_op::join(ss, sep);
}

inline std::string dm_srcinfo_by_fileline(const char *srcfile, int srcline) {
  std::ostringstream oss;
  oss << srcfile << ":" << srcline;
  return oss.str();
}

inline std::string dm_srcinfo_by_backtrace() {
  return std::string("ban backtrace");

  const int MAX_DEPTH = 8;
  const int SKIP_DEPTH = 1;
  const int BT_BUF_SIZE = 128;

  std::vector<std::string> bt;

  int nptrs;
  void *buffer[BT_BUF_SIZE];
  char **strings;
  nptrs = backtrace(buffer, BT_BUF_SIZE);
  strings = backtrace_symbols(buffer, nptrs);
  if (strings == NULL) {
    return "unknown";
  }

  for (int j = SKIP_DEPTH; j < nptrs && j < MAX_DEPTH; j++) {
    bt.push_back(std::string(strings[j]));
  }

  free(strings);

  return dm_join(bt, ",");
}


} // namespace inner

// --------------------------------------------------------------------------------------------------------------------

#define DEBUG_MUTEX_LOCK(dm) dm.lock(__FILE__, __LINE__);
#define DEBUG_MUTEX_UNLOCK(dm) dm.unlock(__FILE__, __LINE__);

// --------------------------------------------------------------------------------------------------------------------

// 业务方可使用这个单例在外部修改一些配置
class debug_mutex_config {
public:
  static debug_mutex_config &instance() {
    static debug_mutex_config *dmc=NULL;
    static std::mutex mu;

    if (dmc == NULL) {
      mu.lock();
      if (dmc == NULL) {
        dmc = new debug_mutex_config();
        dmc->long_wait_acquire_duration = 1000;    // 等太久获取锁的时间阈值
        dmc->long_hold_duration         = 1000;    // 持锁太久的时间阈值
        dmc->print_interval             = 1000;    // 上面两项检查输出间隔
        dmc->logfd                      = stderr;  // 日志输出文件描述符
        dmc->key_prefix                 = "MUTEX"; // 唯一ID前缀
        dmc->log_ctor_flag              = true;    // 锁构造时的日志
        dmc->log_dector_flag            = true;   // 锁析构时的日志
        dmc->log_lock_flag              = false;   // 获取锁的日志
        dmc->log_unlock_flag            = false;   // 释放锁的日志
      }
      fprintf(debug_mutex_config::instance().logfd, "[ERROR] [%llu] new debug_mutex_config instance.\n", inner::dm_tick_msec());
      mu.unlock();
    }
    return *dmc;
  }

public:
  std::uint64_t long_wait_acquire_duration;
  std::uint64_t long_hold_duration;
  std::uint64_t print_interval;
  FILE *logfd;
  std::string key_prefix;
  bool log_ctor_flag;
  bool log_dector_flag;
  bool log_lock_flag;
  bool log_unlock_flag;
};

// --------------------------------------------------------------------------------------------------------------------

class debug_mutex_manager {
public:
  debug_mutex_manager() :
    bacquire_count_(0),
    aacquire_count_(0),
    aunlock_count_(0),
    print_thread_(NULL)
    {
    }

  void before_acquire_lock(const std::string &uk, int tid, const std::string &srcinfo) {
    std::ostringstream oss;
    oss << uk << "_" << tid;
    std::string k=oss.str();

    std::lock_guard<std::mutex> guard(mu_);

    start_print_tmp_debug_if_needed();

    bacquire_count_++;

    std::map<std::string, item>::iterator iter;
    iter = wait_acquire_container_.find(k);
    // exist
    if (iter != wait_acquire_container_.end()) {
      fprintf(debug_mutex_config::instance().logfd, "[ERROR] [%s] [%d] [%llu] one g try acquire lock twice(wait acquire already). - %s\n",
          uk.c_str(), tid, inner::dm_tick_msec(), srcinfo.c_str());
    }

    iter = hold_container_.find(k);
    if (iter != hold_container_.end()) {
      fprintf(debug_mutex_config::instance().logfd, "[ERROR] [%s] [%d] [%llu] one g try acquire lock twice(hold already). - %s\n",
          uk.c_str(), tid, inner::dm_tick_msec(), srcinfo.c_str());
    }

    item v;
    v.time_ = inner::dm_tick_msec();
    v.srcinfo_ = srcinfo;
    wait_acquire_container_[k] = v;
  }

  void after_acquire_lock(const std::string &uk, int tid, const std::string &srcinfo) {
    std::ostringstream oss;
    oss << uk << "_" << tid;
    std::string k=oss.str();

    std::lock_guard<std::mutex> guard(mu_);

    start_print_tmp_debug_if_needed();

    aacquire_count_++;

    std::map<std::string, item>::iterator iter;
    iter = wait_acquire_container_.find(k);
    // not exist
    if (iter == wait_acquire_container_.end()) {
      fprintf(debug_mutex_config::instance().logfd, "[ERROR] [%s] [%d] [%llu] acquired but not in wait acquire container. - %s\n",
          uk.c_str(), tid, inner::dm_tick_msec(), srcinfo.c_str());
    }

    iter = hold_container_.find(k);
    if (iter != hold_container_.end()) {
      fprintf(debug_mutex_config::instance().logfd, "[ERROR] [%s] [%d] [%llu] acquired but in  hold container already. - %s\n",
          uk.c_str(), tid, inner::dm_tick_msec(), srcinfo.c_str());
    }

    wait_acquire_container_.erase(k);
    item v;
    v.time_ = inner::dm_tick_msec();
    v.srcinfo_ = srcinfo;
    hold_container_[k] = v;
  }

  void after_unlock(const std::string &uk, int tid, const std::string &srcinfo) {
    std::ostringstream oss;
    oss << uk << "_" << tid;
    std::string k=oss.str();

    std::lock_guard<std::mutex> guard(mu_);

    start_print_tmp_debug_if_needed();

    aunlock_count_++;

    std::map<std::string, item>::iterator iter;
    iter = hold_container_.find(k);
    if (iter == hold_container_.end()) {
      fprintf(debug_mutex_config::instance().logfd, "[ERROR] [%s] [%d] [%llu] unlock but not in hold container. - %s\n",
          uk.c_str(), tid, inner::dm_tick_msec(), srcinfo.c_str());
    }

    iter = wait_acquire_container_.find(k);
    if (iter != wait_acquire_container_.end()) {
      fprintf(debug_mutex_config::instance().logfd, "[ERROR] [%s] [%d] [%llu] unlock but in wait acquire container already. - %s\n",
          uk.c_str(), tid, inner::dm_tick_msec(), srcinfo.c_str());
    }

    hold_container_.erase(k);
  }

private:
  void start_print_tmp_debug_if_needed() {
    if (print_thread_ == NULL) {
      print_thread_ = new std::thread(&debug_mutex_manager::print_tmp_debug, this);
    }
  }

  void print_tmp_debug() {
    for (; ; ) {
      mu_.lock();

      uint64_t now = inner::dm_tick_msec();

      std::ostringstream oss;
      std::map<std::string, item>::iterator iter;
      bool long_flag;

      long_flag = false;
      oss << "[" << now << "] long wait acquire:";
      for (iter = wait_acquire_container_.begin(); iter != wait_acquire_container_.end(); iter++) {
        uint64_t duration = now - iter->second.time_;
        if (duration > debug_mutex_config::instance().long_wait_acquire_duration) {
          long_flag = true;
          oss << " [" << iter->first << ":(" << duration << "ms) (" << iter->second.srcinfo_ << ")]";
        }
      }
      if (!long_flag) {
        oss << "nope";
      }
      fprintf(debug_mutex_config::instance().logfd, "[DEBUG] %s\n", oss.str().c_str());

      long_flag = false;
      oss.str("");
      oss << "[" << now << "] long hold:";
      for (iter = hold_container_.begin(); iter != hold_container_.end(); iter++) {
        uint64_t duration = now - iter->second.time_;
        if (duration > debug_mutex_config::instance().long_hold_duration) {
          long_flag = true;
          oss << " [" << iter->first << ":(" << duration << "ms) (" << iter->second.srcinfo_ << ")]";
        }
      }
      if (!long_flag) {
        oss << "nope";
      }
      fprintf(debug_mutex_config::instance().logfd, "[DEBUG] %s\n", oss.str().c_str());

      oss.str("");
      oss << "[" << now << "] wait num:" << wait_acquire_container_.size() << ", hold num:" << hold_container_.size()
          << ", count: " << bacquire_count_ << ", "<< aacquire_count_ << ", " << aunlock_count_;
      fprintf(debug_mutex_config::instance().logfd, "[DEBUG] %s\n", oss.str().c_str());

      fflush(debug_mutex_config::instance().logfd);

      mu_.unlock();

      usleep(debug_mutex_config::instance().print_interval * 1000);
    }
  }

private:
  struct item {
    std::uint64_t time_;
    std::string   srcinfo_;
  };

private:
  std::mutex mu_;
  std::map<std::string, item> wait_acquire_container_;
  std::map<std::string, item> hold_container_;
  std::thread *print_thread_;
  std::uint64_t bacquire_count_;
  std::uint64_t aacquire_count_;
  std::uint64_t aunlock_count_;
};

// --------------------------------------------------------------------------------------------------------------------

template<class Dummy>
struct debug_mutex_static {
  static std::atomic<std::uint64_t> total_index_;
  static debug_mutex_manager dmm_;
};

template<class Dummy>
std::atomic<std::uint64_t> debug_mutex_static<Dummy>::total_index_;

template<class Dummy>
debug_mutex_manager debug_mutex_static<Dummy>::dmm_;

class debug_mutex : private debug_mutex_static<void> {
public:
  debug_mutex(const char *srcfile, int srcline) : debug_mutex(inner::dm_srcinfo_by_fileline(srcfile, srcline)) {
  }

  debug_mutex() : debug_mutex(inner::dm_srcinfo_by_backtrace()) {
  }

  debug_mutex(std::string srcinfo) {
    index_ = ++total_index_;

    std::ostringstream oss;
    oss << debug_mutex_config::instance().key_prefix << index_;
    unique_key_ = oss.str();

    ctor_time_ = inner::dm_tick_msec();

    if (debug_mutex_config::instance().log_ctor_flag) {
      fprintf(debug_mutex_config::instance().logfd, "[DEBUG] [%s] [%d] [%llu] debug_mutex(). - %s\n",
              unique_key_.c_str(), inner::dm_gettid(), ctor_time_, srcinfo.c_str());
    }
  }

  ~debug_mutex() {
    std::uint64_t t = inner::dm_tick_msec();
    if (debug_mutex_config::instance().log_dector_flag) {
      fprintf(debug_mutex_config::instance().logfd, "[DEBUG] [%s] [%d] [%llu] ~debug_mutex(). life cost=%llu - %s\n",
              unique_key_.c_str(), inner::dm_gettid(), t, t-ctor_time_, inner::dm_srcinfo_by_backtrace().c_str());
    }
  }

  void lock() {
    lock(inner::dm_srcinfo_by_backtrace());
  }

  void lock(const char *srcfile, int srcline) {
    lock(inner::dm_srcinfo_by_fileline(srcfile, srcline));
  }

  void lock(std::string srcinfo) {
    int tid = inner::dm_gettid();
    std::uint64_t b = inner::dm_tick_msec();

    if (debug_mutex_config::instance().log_lock_flag) {
      fprintf(debug_mutex_config::instance().logfd, "[DEBUG] [%s] [%d] [%llu] > lock(). - %s\n",
              unique_key_.c_str(), tid, b, srcinfo.c_str());
    }
    dmm_.before_acquire_lock(unique_key_, tid, srcinfo);
    core_.lock();
    dmm_.after_acquire_lock(unique_key_, tid, srcinfo);
    std::uint64_t e = inner::dm_tick_msec();
    start_hold_time_ = e;
    if (debug_mutex_config::instance().log_lock_flag) {
      fprintf(debug_mutex_config::instance().logfd, "[DEBUG] [%s] [%d] [%llu] < lock(). acquire cost=%llu - %s\n",
              unique_key_.c_str(), tid, e, e-b, srcinfo.c_str());
    }
  }

  void unlock() {
    unlock(inner::dm_srcinfo_by_backtrace());
  }

  void unlock(const char *srcfile, int srcline) {
    unlock(inner::dm_srcinfo_by_fileline(srcfile, srcline));
  }

  void unlock(std::string srcinfo) {
    int tid = inner::dm_gettid();
    std::uint64_t b = inner::dm_tick_msec();
    if (debug_mutex_config::instance().log_unlock_flag) {
      fprintf(debug_mutex_config::instance().logfd, "[DEBUG] [%s] [%d] [%llu] > unlock(). hold cost=%llu - %s\n",
              unique_key_.c_str(), tid, b, b-start_hold_time_, srcinfo.c_str());
    }
    core_.unlock();
    dmm_.after_unlock(unique_key_, tid, srcinfo);
    if (debug_mutex_config::instance().log_unlock_flag) {
      fprintf(debug_mutex_config::instance().logfd, "[DEBUG] [%s] [%d] [%llu] < unlock(). release cost=%llu - %s\n",
              unique_key_.c_str(), tid, b, inner::dm_tick_msec()-b, srcinfo.c_str());
    }
  }

private:
  chef::recursive_mutex core_;
  std::uint64_t ctor_time_;
  std::uint64_t start_hold_time_;

  std::uint64_t index_;
  std::string unique_key_;
};

// --------------------------------------------------------------------------------------------------------------------

class debug_lock_guard {
public:
  debug_lock_guard(debug_mutex &dm, const char *srcfile, int srcline) : dm_(dm) {
    dm_.lock(srcfile, srcline);
  }

  debug_lock_guard(debug_mutex &dm) : dm_(dm) {
    dm_.lock(inner::dm_srcinfo_by_backtrace());
  }

  ~debug_lock_guard() {
    dm_.unlock();
  }

private:
  debug_mutex &dm_;
};

}

#endif

