/**
 * @file   redis_backend.h/redis_backend.cc
 * @deps   hirdis
 *
 * @author
 *   chef <191201771@qq.com>
 *     -initial release 2016-08-31
 *
 * @brief
 *   场景：将程序中某些数据以kv的形式在redis中做镜像，程序重启后能一次性全部load回来。
 *   value可以是普通字符串，也可以是json序列化后的字符串。基本满足需求～
 *
 *   hset hdel做成异步是方便在各业务线程中直接调用，避免io等原因阻塞业务线程。
 *   后台长链接断开后，会缓存住后续添加的异步任务，重连上一次性执行完。
 *   调用其他接口前，需先调用start。其他所有接口都是线程安全的。
 *
 * @TODO
 *   增加redis长链接/堆积任务状态回调，或状态检查接口给上层使用者
 *
 */

#ifndef _CHEF_BASE_REDIS_BACKEND_H_
#define _CHEF_BASE_REDIS_BACKEND_H_

#include "task_thread.h"
#include "noncopyable.hpp"
#include "env.hpp"
#include <hiredis/hiredis.h>
#include <string>
#include <vector>
#include <deque>

namespace chef {

  class redis_backend : chef::noncopyable {
    public:
      /**
       * @param ip                  redis ip
       * @param port                redis port
       * @param container_name      容器名
       * @param connect_timeout_sec 连接redis超时时间，单位秒
       * @param ping_interval_sec   ping间隔时间，单位秒
       *
       */
      redis_backend(const std::string &ip,
                    int port,
                    const std::string &container_name,
                    int connect_timeout_sec = 3,
                    int ping_interval_sec = 1);

      /**
       * 开启后台线程，建立redis长链接
       *
       * @return 0 成功 -1 失败
       *
       */
      int start();

      /**
       * 同步返回执行结果
       *
       * @param keys   传出参数 container中的所有key
       * @param values 传出参数 container中的所有value
       * key，value通过下标一一对应
       *
       * @return 0 成功 -1 失败
       *
       */
      int hgetall(std::vector<std::string> &keys, std::vector<std::string> &values);

      /**
       * 异步设置，失败后内部会自动重试
       *
       */
      void async_hset(const std::string &key, const std::string &value);

      /**
       * 异步删除，失败后内部会自动重试
       *
       */
      void async_hdel(const std::string &key);

      /**
       * 测试是否能ping通redis
       * 每次调用都新建一条链接，ping完后关闭
       *
       * @return 0 成功 -1 失败
       *
       */
      int ping();

    private:
      /**
       * 简易封装redisConnectWithTimeout
       *
       * @param 成功返回redisContext指针，失败返回NULL
       *
       */
      redisContext *connect();

      /**
       * 以下函数只能在backend_thread_中调用
       *
       */
      void connect_in_thread();
      void ping_in_thread();
      void hset_in_thread(const std::string &key, const std::string &value);
      void hdel_in_thread(const std::string &key);

    private:
      enum task_type {
        TASK_TYPE_HSET,
        TASK_TYPE_HDEL,
      };

      struct redis_task {
        task_type   type_;
        std::string key_;
        std::string value_;

        redis_task(task_type t, const std::string &k, const std::string &v)
          : type_(t)
          , key_(k)
          , value_(v)
        {}
        redis_task(task_type t, const std::string &k)
          : type_(t)
          , key_(k)
          , value_(std::string())
        {}
      };

    private:
      typedef chef::shared_ptr<redis_task> redis_task_ptr;
      typedef std::deque<redis_task_ptr>   redis_task_deque;

    private:
      std::string        ip_;
      int                port_;
      std::string        container_name_;
      int                connect_timeout_sec_;
      int                ping_interval_sec_;
      chef::task_thread  backend_thread_;
      redisContext      *backend_context_;
      bool               started_;
      redis_task_deque   undone_tasks_;
  };

}  // namespace chef

#endif // _CHEF_BASE_REDIS_BACKEND_H_
