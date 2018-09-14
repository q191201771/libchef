/**
 * @tag      v1.3.6
 * @file     chef_os_exec_op.hpp
 * @deps     nope
 * @platform linux/macos/xxx
 *
 * @author
 *   chef <191201771@qq.com>
 *     -initial release 2016-08-31
 *
 * @brief    执行shell命令
 *
     ```
     std::time_t time = std::time(0);
     std::string filename = "/tmp/os_op_test." + std::to_string(time);
     std::string touch = "echo \"hello\nworld\" >> " + filename;
     std::vector<std::string> touch_result = chef::os_exec_op::run_command(touch);
     //assert(touch_result.size() == 0);
     std::string cat = "cat " + filename;
     std::vector<std::string> cat_result = chef::os_exec_op::run_command(cat);
     //assert(cat_result.size() == 2);
     //assert(cat_result[0] == "hello");
     //assert(cat_result[1] == "world");
     std::string rm = "rm " + filename;
     chef::os_exec_op::run_command(rm);
     ```
 *
 */

#ifndef _CHEF_BASE_OS_EXEC_OP_H_
#define _CHEF_BASE_OS_EXEC_OP_H_
#pragma once

#include <string>
#include <vector>

namespace chef {

  class os_exec_op {
    public:
      /**
       * 执行shell命令，返回包含每行执行结果的数组
       *
       */
      static std::vector<std::string> run_command(const std::string &cmd);

    private:
      os_exec_op();
      os_exec_op(const os_exec_op &);
      os_exec_op &operator=(const os_exec_op &);

  }; // class os_exec_op

} // namespace chef





///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// @NOTICE 该分隔线以上部分为该模块的接口，分割线以下部分为对应的实现





#include <string.h>
#include <stdio.h>

namespace chef {

  inline std::vector<std::string> os_exec_op::run_command(const std::string &cmd) {
    std::vector<std::string> result;

    FILE *fp = popen(cmd.c_str(), "r");
    if (fp == NULL) {
      fprintf(stderr, "popen(\"%s\") fail.\n", cmd.c_str());
      return result;
    }

    const int MAX_LEN = 8192;
    char line[MAX_LEN] = {0};
    for (; fgets(line, MAX_LEN, fp); ) {
      size_t rend = strlen(line) - 1;
      if (line[rend] == '\n') {
        line[rend] = '\0';
      }
      result.push_back(line);
    }

    pclose(fp);

    return result;
  }

} // namespace chef

#endif // _CHEF_BASE_OS_EXEC_OP_H_
