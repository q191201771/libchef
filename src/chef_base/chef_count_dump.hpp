/**
 * @tag      v1.5.15
 * @file     chef_count_dump.hpp
 * @deps     chef_env.hpp | chef_filepath_op.hpp
 * @platform linux | macos | xxx
 *
 * @author
 *   chef <191201771@qq.com>
 *     - initial release xxxx-xx-xx
 *
 * @brief
 *   提供两个类：
 *   - class multi_tag_counter 在各种线程模型及使用场景下高效的对多个tag进行计数（打点）
 *   - class multi_tag_count_dumper 支持定时将multi_tag_counter内部计数落盘，观察打点变化
 *
 */

#ifndef _CHEF_BASE_COUNT_DUMP_HPP_
#define _CHEF_BASE_COUNT_DUMP_HPP_
#pragma once

#include "chef_env.hpp"
#include "chef_filepath_op.hpp"
#include <assert.h>
#include <stdint.h>
#include <stdio.h>
#include <map>
#include <string>
#include <vector>

namespace chef {

  class multi_tag_counter {
    public:
      enum multi_tag_counter_type {
        MULTI_TAG_COUNTER_FREE_LOCK, // 无锁模式,适用于单线程环境使用
        MULTI_TAG_COUNTER_ATOMIC,    // 只锁计数器,适用于所有tag在一开始就固定下来
        MULTI_TAG_COUNTER_MUTEX,     // 锁模式,适用于tag不固定
      };

    public:
      explicit multi_tag_counter(multi_tag_counter_type type) : type_(type) {}
      ~multi_tag_counter() {}

    public:
      /**
       * 添加tag会把count置0
       * @NOTICE
       *   如果是MULTI_TAG_COUNTER_ATOMIC,则在开始计数后不应该再调用操作tag的接口
       *   如果是其他两种模式,可以直接使用计数接口,添加tag是非必须的
       *
       */
      void add_tag(const std::string &tag);
      void add_tags(const std::vector<std::string> &tags);
      void del_tag(const std::string &tag);

      /**
       * @NOTICE
       *   如果是MULTI_TAG_COUNTER_ATOMIC,则tag不存在时返回false
       *   如果是其他两种模式,则永远返回true. count会先初始化为0再做函数调用对应的操作
       *
       */
      bool increment(const std::string &tag);
      bool decrement(const std::string &tag);
      bool add_count(const std::string &tag, int64_t num);
      bool del_count(const std::string &tag, int64_t num);
      bool set_count(const std::string &tag, int64_t num);

      // 获取单个tag的计数,如果tag不存在则返回false
      bool get_tag_count(const std::string &tag, int64_t *num /* out */);
      std::map<std::string, int64_t> get_tags_count();

    private:
      multi_tag_counter(const multi_tag_counter &);
      multi_tag_counter &operator=(const multi_tag_counter &);

    private:
      typedef std::map<std::string, int64_t> TAG2COUNT;
      typedef std::map<std::string, chef::atomic<int64_t> > TAG2ATOMIC_COUNT;

    private:
      multi_tag_counter_type type_;
      TAG2COUNT              tag2count_;
      TAG2ATOMIC_COUNT       tag2atomic_count_;
      chef::mutex            mutex_;

  }; // class multi_tag_counter

  class multi_tag_count_dumper {
    public:
      enum multi_tag_count_dump_type {
        MULTI_TAG_COUNT_DUMP_TYPE_OVERWRITE,
        MULTI_TAG_COUNT_DUMP_TYPE_APPEND
      };
    public:
      multi_tag_count_dumper(multi_tag_counter *mtc, int interval_ms, uint32_t num_per_line, const std::string &filename,
                             multi_tag_count_dump_type type=MULTI_TAG_COUNT_DUMP_TYPE_OVERWRITE)
        : mtc_(mtc)
        , interval_ms_(interval_ms)
        , num_per_line_(num_per_line)
        , filename_(filename)
        , exit_flag_(false)
        , type_(type)
      { assert(mtc); }

      ~multi_tag_count_dumper();

    public:
      void start();

    private:
      void run_in_thread();
      void dump2disk();
      std::string styled_stringify();

    private:
      multi_tag_count_dumper(const multi_tag_count_dumper &);
      multi_tag_count_dumper &operator=(const multi_tag_count_dumper &);

    private:
      multi_tag_counter              *mtc_;
      int                            interval_ms_;
      uint32_t                       num_per_line_;
      std::string                    filename_;
      chef::shared_ptr<chef::thread> thread_;
      bool                           exit_flag_;
      multi_tag_count_dump_type      type_;

  }; // class multi_tag_count_dumper

} // namespace chef





///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// @NOTICE 该分隔线以上部分为该模块的接口，分割线以下部分为对应的实现





#include <ctime>
#include <sstream>

namespace chef {

  #define LOCK_IF_NEEDED(t, m) { if ((t) == MULTI_TAG_COUNTER_MUTEX) m.lock(); }
  #define UNLOCK_IF_NEEDED(t, m) { if ((t) == MULTI_TAG_COUNTER_MUTEX) m.unlock(); }

  inline void multi_tag_counter::add_tag(const std::string &tag) {
    if (type_ == MULTI_TAG_COUNTER_ATOMIC) {
      tag2atomic_count_[tag] = 0;
      return;
    }
    LOCK_IF_NEEDED(type_, mutex_);
    tag2count_[tag] = 0;
    UNLOCK_IF_NEEDED(type_, mutex_);
  }

  inline void multi_tag_counter::add_tags(const std::vector<std::string> &tags) {
    std::vector<std::string>::const_iterator iter = tags.begin();
    for (; iter != tags.end(); ++iter) {
      add_tag(*iter);
    }
  }

  inline void multi_tag_counter::del_tag(const std::string &tag) {
    if (type_ == MULTI_TAG_COUNTER_ATOMIC) {
      assert(0);
      tag2atomic_count_.erase(tag);
      return;
    }
    LOCK_IF_NEEDED(type_, mutex_);
    tag2count_.erase(tag);
    UNLOCK_IF_NEEDED(type_, mutex_);
  }

  inline bool multi_tag_counter::increment(const std::string &tag) {
    if (type_ == MULTI_TAG_COUNTER_ATOMIC) {
      TAG2ATOMIC_COUNT::iterator iter = tag2atomic_count_.find(tag);
      if (iter == tag2atomic_count_.end()) {
        return false;
      }
      iter->second++;
      return true;
    }
    LOCK_IF_NEEDED(type_, mutex_);
    tag2count_[tag]++;
    UNLOCK_IF_NEEDED(type_, mutex_);
    return true;
  }

  inline bool multi_tag_counter::decrement(const std::string &tag) {
    if (type_ == MULTI_TAG_COUNTER_ATOMIC) {
      TAG2ATOMIC_COUNT::iterator iter = tag2atomic_count_.find(tag);
      if (iter == tag2atomic_count_.end()) {
        return false;
      }
      iter->second--;
      return true;
    }
    LOCK_IF_NEEDED(type_, mutex_);
    tag2count_[tag]--;
    UNLOCK_IF_NEEDED(type_, mutex_);
    return true;
  }

  inline bool multi_tag_counter::add_count(const std::string &tag, int64_t num) {
    if (type_ == MULTI_TAG_COUNTER_ATOMIC) {
      TAG2ATOMIC_COUNT::iterator iter = tag2atomic_count_.find(tag);
      if (iter == tag2atomic_count_.end()) {
        return false;
      }
      iter->second += num;
      return true;
    }
    LOCK_IF_NEEDED(type_, mutex_);
    tag2count_[tag] += num;
    UNLOCK_IF_NEEDED(type_, mutex_);
    return true;
  }

  inline bool multi_tag_counter::del_count(const std::string &tag, int64_t num) {
    if (type_ == MULTI_TAG_COUNTER_ATOMIC) {
      TAG2ATOMIC_COUNT::iterator iter = tag2atomic_count_.find(tag);
      if (iter == tag2atomic_count_.end()) {
        return false;
      }
      iter->second -= num;
      return true;
    }
    LOCK_IF_NEEDED(type_, mutex_);
    tag2count_[tag] -= num;
    UNLOCK_IF_NEEDED(type_, mutex_);
    return true;
  }

  inline bool multi_tag_counter::set_count(const std::string &tag, int64_t num) {
    if (type_ == MULTI_TAG_COUNTER_ATOMIC) {
      TAG2ATOMIC_COUNT::iterator iter = tag2atomic_count_.find(tag);
      if (iter == tag2atomic_count_.end()) {
        return false;
      }
      iter->second = num;
      return true;
    }
    LOCK_IF_NEEDED(type_, mutex_);
    tag2count_[tag] = num;
    UNLOCK_IF_NEEDED(type_, mutex_);
    return true;
  }

  inline bool multi_tag_counter::get_tag_count(const std::string &tag, int64_t *num) {
    if (type_ == MULTI_TAG_COUNTER_ATOMIC) {
      TAG2ATOMIC_COUNT::iterator iter = tag2atomic_count_.find(tag);
      if (iter == tag2atomic_count_.end()) {
        return false;
      }
      *num = iter->second;
      return true;
    }
    LOCK_IF_NEEDED(type_, mutex_);
    bool ret = false;
    TAG2COUNT::iterator iter = tag2count_.find(tag);
    if (iter != tag2count_.end()) {
      *num = iter->second;
      ret = true;
    }
    UNLOCK_IF_NEEDED(type_, mutex_);
    return ret;
  }

  inline std::map<std::string, int64_t> multi_tag_counter::get_tags_count() {
    TAG2COUNT ret;
    if (type_ == MULTI_TAG_COUNTER_ATOMIC) {
      TAG2ATOMIC_COUNT::iterator iter = tag2atomic_count_.begin();
      for (; iter != tag2atomic_count_.end(); ++iter) {
        ret[iter->first] = iter->second;
      }
      return ret;
    }
    LOCK_IF_NEEDED(type_, mutex_);
    ret = tag2count_;
    UNLOCK_IF_NEEDED(type_, mutex_);
    return ret;
  }

  inline multi_tag_count_dumper::~multi_tag_count_dumper() {
    exit_flag_ = true;
    if (thread_) {
      thread_->join();
      thread_.reset();
      dump2disk();
    }
  }

  inline void multi_tag_count_dumper::start() {
    thread_ = chef::make_shared<chef::thread>(chef::bind(&multi_tag_count_dumper::run_in_thread, this));
  }

  inline void multi_tag_count_dumper::run_in_thread() {
    while (!exit_flag_) {
      dump2disk();
      chef::this_thread::sleep_for(chef::chrono::milliseconds(interval_ms_));
    }
  }

  inline void multi_tag_count_dumper::dump2disk() {
    std::ostringstream ss;

    std::time_t now = std::time(NULL);
    ss << "------------------------------------------\n"
       << "chef count dump - " << std::asctime(std::localtime(&now))
       << "----\n\n"
       << styled_stringify()
       << "------------------------------------------\n";

    if (type_ == MULTI_TAG_COUNT_DUMP_TYPE_OVERWRITE) {
      filepath_op::write_file(filename_+".tmp", ss.str());
      filepath_op::rename(filename_+".tmp", filename_);
    } else {
      ss << "\n\n";
      filepath_op::write_file(filename_, ss.str(), true);
    }
  }

  inline std::string multi_tag_count_dumper::styled_stringify() {
    std::map<std::string, int64_t> tag2count = mtc_->get_tags_count();

    std::ostringstream ss;
    std::map<std::string, int64_t>::iterator iter = tag2count.begin();
    uint32_t count = 1;
    for (; iter != tag2count.end(); ++iter, ++count) {
      ss << iter->first << ": " << iter->second;
      if ((count % num_per_line_ == 0) ||
          (count == tag2count.size())
      ) {
        ss << "\n";
      } else {
        ss << " | ";
      }
    }
    return ss.str();
  }

  #undef LOCK_IF_NEEDED
  #undef UNLOCK_IF_NEEDED

} // namespace chef

#endif // _CHEF_BASE_COUNT_DUMP_HPP_
