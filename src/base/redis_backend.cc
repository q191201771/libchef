#include "redis_backend.h"
#include <assert.h>

namespace chef {

  redis_backend::redis_backend(const std::string &ip,
    int port,
    const std::string &container_name,
    int connect_timeout_sec,
    int ping_interval_sec
  )
    : ip_(ip)
    , port_(port)
    , container_name_(container_name)
    , connect_timeout_sec_(connect_timeout_sec)
    , ping_interval_sec_(ping_interval_sec)
    , backend_thread_(std::string("redis_backend"))
    , backend_context_(NULL)
    , started_(false)
  {
  }

  int redis_backend::start() {
    backend_context_ = connect();
    if (backend_context_ == NULL) {
      return -1;
    }
    backend_thread_.start();
    backend_thread_.add(std::bind(&redis_backend::ping_in_thread, this));
    started_ = true;
    return 0;
  }

  int redis_backend::ping() {
    redisContext *context = connect();
    if (context == NULL) {
      return -1;
    }
    redisReply *reply = (redisReply *)redisCommand(context, "PING");
    if (reply == NULL) {
      redisFree(context);
      return -1;
    }
    freeReplyObject(reply);
    return 0;
  }

  int redis_backend::hgetall(std::vector<std::string> &keys, std::vector<std::string> &values) {
    redisContext *context = connect();
    if (context == NULL) {
      return -1;
    }
    int ret = -1;
    redisReply *reply = NULL;
    do {
      reply = (redisReply *)redisCommand(context, "HGETALL %s", container_name_.c_str());
      if (reply == NULL || reply->type != REDIS_REPLY_ARRAY) {
        ret = -1;
        break;
      }
      if (reply->elements <= 0) {
        ret = 0;
        break;
      }
      if (reply->elements % 2 != 0) {
        ret = -1;
        break;
      }
      for (size_t i = 0; i < reply->elements; i++) {
        std::string redis_item(reply->element[i]->str, reply->element[i]->len);
        if (i % 2 == 0) {
          keys.push_back(redis_item);
        } else {
          values.push_back(redis_item);
        }
      }
      ret = 0;

    } while(0);

    if (reply) {
      freeReplyObject(reply);
    }
    redisFree(context);
    return ret;
  }

  void redis_backend::async_hset(const std::string &key, const std::string &value) {
    assert(started_);
    backend_thread_.add(std::bind(&redis_backend::hset_in_thread, this, key, value));
  }

  void redis_backend::async_hdel(const std::string &key) {
    assert(started_);
    backend_thread_.add(std::bind(&redis_backend::hdel_in_thread, this, key));
  }

  void redis_backend::hset_in_thread(const std::string &key, const std::string &value) {
    if (backend_context_ == NULL) {
      undone_tasks_.push_back(std::make_shared<redis_task>(TASK_TYPE_HSET, key, value));
      return;
    }
    redisReply *reply = (redisReply *)redisCommand(
      backend_context_,
      "HSET %s %s %s",
      container_name_.c_str(),
      key.c_str(),
      value.c_str()
    );
    if (reply == NULL) {
      undone_tasks_.push_back(std::make_shared<redis_task>(TASK_TYPE_HSET, key, value));
      return;
    }
    freeReplyObject(reply);
  }

  void redis_backend::hdel_in_thread(const std::string &key) {
    if (backend_context_ == NULL) {
      undone_tasks_.push_back(std::make_shared<redis_task>(TASK_TYPE_HDEL, key));
      return;
    }
    redisReply *reply = (redisReply *)redisCommand(
      backend_context_,
      "HDEL %s %s",
      container_name_.c_str(),
      key.c_str()
    );
    if (reply == NULL) {
      undone_tasks_.push_back(std::make_shared<redis_task>(TASK_TYPE_HDEL, key));
      return;
    }
    freeReplyObject(reply);
  }

  redisContext *redis_backend::connect() {
    struct timeval timeout = {connect_timeout_sec_, 0};
    redisContext *context = redisConnectWithTimeout(ip_.c_str(), port_, timeout);
    if (context && context->err) {
      redisFree(context);
      context = NULL;
    }
    return context;
  }

  void redis_backend::connect_in_thread() {
    backend_context_ = connect();
    if (backend_context_) {
      while (!undone_tasks_.empty()) {
        auto t = undone_tasks_.front();
        undone_tasks_.pop_front();
        switch (t->type_) {
          case TASK_TYPE_HSET:
            hset_in_thread(t->key_, t->value_);
            break;
          case TASK_TYPE_HDEL:
            hdel_in_thread(t->key_);
            break;
        }
      }
    }
  }

  void redis_backend::ping_in_thread() {
    if (backend_context_ == NULL) {
      connect_in_thread();
      backend_thread_.add(std::bind(&redis_backend::ping_in_thread, this), ping_interval_sec_);
      return;
    }

    redisReply *reply = (redisReply *)redisCommand(backend_context_, "PING");
    if (reply == NULL) {
      redisFree(backend_context_);
      backend_context_ = NULL;
    } else {
      freeReplyObject(reply);
    }

    backend_thread_.add(std::bind(&redis_backend::ping_in_thread, this), ping_interval_sec_);
  }

} // namespace chef
