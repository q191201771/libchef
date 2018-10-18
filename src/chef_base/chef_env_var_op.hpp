/**
 * @tag      v1.4.11
 * @file     chef_env_var_op.hpp
 * @deps     nope
 * @platform linux | macos | xxx
 *
 * @author
 *   chef <191201771@qq.com>
 *     -initial release xxxx-xx-xx
 *
 * @brief    读写系统环境变量
 *
     ```
     // 获取用户根目录
     succ = chef::env_var_op::getenv("HOME", &v);
     //assert(succ);
     //std::cout << succ << "," << v << std::endl;
     // 获取不存在的环境变量
     std::string k = std::string("chef_env_var_op_test_k_") + std::to_string(std::time(0));
     succ = chef::env_var_op::getenv(k, &v);
     //assert(!succ);
     // 删除不存在的环境变量
     succ = chef::env_var_op::unsetenv(k);
     //assert(succ);
     // 设置不存在的环境变量
     succ = chef::env_var_op::setenv(k, "111");
     //assert(succ);
     // 获取存在的环境变量
     succ = chef::env_var_op::getenv(k, &v);
     //assert(succ && v == "111");
     // 删除存在的环境变量
     succ = chef::env_var_op::unsetenv(k);
     //assert(succ);
     // 获取已删除的环境变量
     succ = chef::env_var_op::getenv(k, &v);
     //assert(!succ);
     ```
 *
 */

#ifndef _CHEF_BASE_ENV_VAR_OP_HPP_
#define _CHEF_BASE_ENV_VAR_OP_HPP_
#pragma once

#include <string>
#include <stdlib.h>

namespace chef {

  class env_var_op {
    public:
      // 存在返回true，不存在返回false
      static bool getenv(const std::string &k, std::string *v);

      // 设置成功返回true，失败返回false
      // @NOTICE 如果设置前已经存在，将直接覆盖
      static bool setenv(const std::string &k, const std::string &v);

      // 删除环境变量
      static bool unsetenv(const std::string &k);

    private:
      env_var_op();
      env_var_op(const env_var_op &);
      env_var_op &operator=(const env_var_op &);

  }; // class env_var_op

} // namespace chef





///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// @NOTICE 该分隔线以上部分为该模块的接口，分割线以下部分为对应的实现





namespace chef {

inline bool env_var_op::getenv(const std::string &k, std::string *v) {
  char *p = ::getenv(k.c_str());
  if (!p) { return false; }

  if (v) { *v = p; }

  return true;
}

inline bool env_var_op::setenv(const std::string &k, const std::string &v) {
  return ::setenv(k.c_str(), v.c_str(), 1) == 0;
}

inline bool env_var_op::unsetenv(const std::string &k) {
  return ::unsetenv(k.c_str()) == 0;
}

} // namespace chef

#endif // _CHEF_BASE_ENV_VAR_OP_HPP_
