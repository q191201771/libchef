/**
 * @file   dump.h/dump.cc
 * @deps   nope
 *
 * @author
 *   chef <191201771@qq.com>
 *     -initial release xxxx-xx-xx
 *
 * @brief
 *   每秒将加入其中的tag->num KV结构写入文件中。
 *   可通过
 *   $watch -n 1 'cat xxx.dump'
 *   监控服务器上一些数值变化(xxx.dump为文件名)。
 *
 */

#ifndef _CHEF_BASE_COUNT_DUMP_H_
#define _CHEF_BASE_COUNT_DUMP_H_

#include "env.hpp"
#include <stdio.h>
#include <stdint.h>
#include <map>
#include <string>
#include <vector>

namespace chef {

  class count_dump {
    private:
      enum count_dump_type {
        COUNT_DUMP_TYPE_MUTABLE_TAGS,  /// 对tags的map容器读写加锁，可随时添加新tag。
        COUNT_DUMP_TYPE_IMMUTABLE_TAGS, /// 对tags的map容器不加锁，初始化以后不能再添加新tag，性能较COUNT_DUMP_TYPE_MUTABLE_TAGS更高。
      };

    public:
      count_dump();
      ~count_dump();

      /**
       * 初始化，开启dump线程，每<dump_interval_ms>刷新一次文件
       *
       * @NOTICE 由于没有指定初始tags，所以只能使用COUNT_DUMP_TYPE_MUTABLE_TAGS
       *
       */
      void init(const std::string &filename, int32_t dump_interval_ms=1000);

      /**
       * 重载init
       *
       * @param         filename dump文件名
       * @param             tags 初始化tags，使用COUNT_DUMP_TYPE_IMMUTABLE_TAGS，后续不能再动态添加新tag
       * @param dump_interval_ms ~
       *
       */
      void init_with_constant_tags(const std::string &filename, const std::vector<std::string> &tags,
                                   int32_t dump_interval_ms=1000);

      /**
       * @return 0 成功 -1 失败
       *
       * if COUNT_DUMP_TYPE_MUTABLE_TAGS:  如果<tag>不存在，添加新tag，设置为<num>，如果<tag>已存在，则已有num再加上<num>。
       * if COUNT_DUMP_TYPE_IMMUTABLE_TAGS: return -1（increment，decrement，del接口相同）
       *
       */
      int add(const std::string &tag, int num);

      /**
       * 等价于 add(tag, 1);
       *
       */
      int increment(const std::string &tag);

      /**
       * 等价于 add(tag, -1);
       *
       */
      int decrement(const std::string &tag);

      /**
       * 等价于 add(tag, -num);
       *
       */
      int del(const std::string &tag, int num);

      /**
       * 将<tag>对应的num置为0
       *
       * 当tag之前不存在时， if COUNT_DUMP_TYPE_MUTABLE_TAGS:  将tag加入并初始化为0
       *                  if COUNT_DUMP_TYPE_IMMUTABLE_TAGS: return -1
       *
       */
      int reset(const std::string &tag);

      /**
       * 将所有tag都置为0
       *
       */
      void reset();

      /**
       * 返回所有kv对，供其他模块使用
       *
       */
      std::map<std::string, int> kv();

    private:
      void run_in_thread_();

    private:
      enum {
//        DUMP_INTERVAL_MS    = 1000,
        NUM_OF_TAG_PER_LINE = 5,
      };

    private:
      typedef std::map<std::string, int>                                             tag2num;
      typedef std::map<std::string, int>::iterator                                   tag2num_iterator;
      typedef std::map<std::string, chef::shared_ptr<chef::atomic<int> > >           tag2atomic_num;
      typedef std::map<std::string, chef::shared_ptr<chef::atomic<int> > >::iterator tag2atomic_num_iterator;
      typedef chef::shared_ptr<chef::thread>                                         thread_ptr;

    private:
      count_dump_type type_;
      std::string     filename_;
      int32_t         dump_interval_ms_;
      tag2num         tag2num_;
      tag2atomic_num  tag2atomic_num_;
      thread_ptr      thread_;
      bool            exit_flag_;
      chef::mutex     mutex_;
  };

} // namespace chef

#endif // _CHEF_BASE_COUNT_DUMP_H_
