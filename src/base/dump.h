/**
 * @file   dump.h/dump.cc
 * @deps   nope
 *
 * @author
 *   chef <191201771@qq.com>
 *     -initial release 2016-08-31
 *
 * @brief
 *   每秒将加入其中的tag->num KV结构写入文件中。
 *   可通过
 *   $watch -n 1 'cat xxx.dump'
 *   监控服务器上一些数值变化(xxx.dump为文件名)。
 *
 */

#ifndef _CHEF_BASE_DUMP_H_
#define _CHEF_BASE_DUMP_H_

#include "env.hpp"
#include <stdio.h>
#include <map>
#include <string>
#include <vector>

namespace chef {

  class dump {
    public:
      dump();
      ~dump();

      /**
       * 见 void dump::init(const std::string &filename, const std::vector<std::string> &tags);
       *
       */
      void init(const std::string &filename);

      /**
       * 初始化，开启dump线程，每[DUMP_INTERVAL_MS]刷新一次文件
       *
       * @param filename dump文件名
       * @param tags
       *   可选，提前初始化一些tag，即使这些tag后续没有操作，也会有一个num=0的记录
       *
       */
      void init(const std::string &filename, const std::vector<std::string> &tags);

      /**
       * 如果[tag]不存在，设置为[num]，如果[tag]已存在，则已有num再加上[num]。
       *
       */
      void add(const std::string &tag, int num);

      /**
       * 等价于 add(tag, 1);
       *
       */
      void increment(const std::string &tag);

      /**
       * 等价于 add(tag, -1);
       *
       */
      void decrement(const std::string &tag);

      /**
       * 等价于 add(tag, -num);
       *
       */
      void del(const std::string &tag, int num);

      /**
       * 将[tag]对应的num置为0
       *
       */
      void reset(const std::string &tag);

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
      void run_in_thread();

    private:
      enum {
        DUMP_INTERVAL_MS    = 1000,
        NUM_OF_TAG_PER_LINE = 5,
      };

    private:
      typedef std::map<std::string, int>   tag2num;
      typedef chef::shared_ptr<chef::thread> thread_ptr;

      std::string  filename_;
      tag2num      tag2num_;
      thread_ptr   thread_;
      bool         exit_flag_;
      chef::mutex   mutex_;
  };

} // namespace chef

#endif // _CHEF_BASE_DUMP_H_
