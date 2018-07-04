/**
 * @file     chef_count_dump.hpp[_impl]
 * @deps     chef_env.hpp | chef_noncopyable.hpp | chef_filepath_op.hpp[_impl]
 * @platform linux | macos | xxx
 *
 * @author
 *   chef <191201771@qq.com>
 *     -initial release xxxx-xx-xx
 *
 * @brief
 *   在各种线程及场景下高效的对多个tag进行计数
 *   同时支持定时将计数落盘
 *   $watch -n 1 'cat xxx.txt'
 *
 */

#ifndef _CHEF_BASE_COUNT_DUMP_H_
#define _CHEF_BASE_COUNT_DUMP_H_
#pragma once

#include "chef_env.hpp"
#include "chef_noncopyable.hpp"
#include <stdio.h>
#include <stdint.h>
#include <map>
#include <string>
#include <vector>

namespace chef {

  class multi_tag_counter : public chef::noncopyable {
    public:
      enum multi_tag_counter_type {
        MULTI_TAG_COUNTER_FREE_LOCK, // 无锁模式,适用于单线程环境使用
        MULTI_TAG_COUNTER_ATOMIC,    // 只锁计数器,适用于所有tag在一开始就固定下来
        MULTI_TAG_COUNTER_MUTEX,     // 锁模式,适用于tag不固定
      };

    public:
      multi_tag_counter(multi_tag_counter_type type) : type_(type) {}
      ~multi_tag_counter() {}

      // 添加tag会把count置0
      // @NOTICE
      //   如果是MULTI_TAG_COUNTER_ATOMIC,则在开始计数后不应该再调用操作tag的接口
      //   如果是其他两种模式,可以直接使用计数接口,添加tag是非必须的
      void add_tag(const std::string &tag);
      void add_tags(const std::vector<std::string> &tags);
      void del_tag(const std::string &tag);

      // @NOTICE
      //   如果是MULTI_TAG_COUNTER_ATOMIC,则tag不存在时返回false
      //   如果是其他两种模式,则永远返回true. count会先初始化为0再做函数调用对应的操作
      bool increment(const std::string &tag);
      bool decrement(const std::string &tag);
      bool add_count(const std::string &tag, int64_t num);
      bool del_count(const std::string &tag, int64_t num);
      bool set_count(const std::string &tag, int64_t num);

      // 获取单个tag的计数,如果tag不存在则返回false
      bool get_tag_count(const std::string &tag, int64_t *num /* out */);
      std::map<std::string, int64_t> get_tags_count();

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
      multi_tag_count_dumper(multi_tag_counter *mtc, int interval_ms, uint32_t num_per_line, const std::string &filename)
        : mtc_(mtc)
        , interval_ms_(interval_ms)
        , num_per_line_(num_per_line)
        , filename_(filename)
        , exit_flag_(false)
      {}
      ~multi_tag_count_dumper();

      void start();

    private:
      void run_in_thread();
      void dump2disk();
      std::string styled_stringify();

    private:
      multi_tag_counter              *mtc_;
      int                            interval_ms_;
      uint32_t                       num_per_line_;
      std::string                    filename_;
      chef::shared_ptr<chef::thread> thread_;
      bool                           exit_flag_;

  }; // class multi_tag_count_dumper

} // namespace chef

#include "chef_count_dump_impl.hpp"

#endif // _CHEF_BASE_COUNT_DUMP_H_
