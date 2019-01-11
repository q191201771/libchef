/**
 * @license  this file is a part of libchef. more info see https://github.com/q191201771/libchef
 * @tag      v1.9.17
 * @file     chef_os_exec_op.hpp
 * @deps     nope
 * @platform linux/macos/xxx
 *
 * @author
 *   chef <191201771@qq.com>
 *     -initial release 2016-08-31
 *
 * @brief    开启子进程执行shell命令，并阻塞式等待结果
 *
     ```
     bool succ;
     int exit_status;
     std::string filename = std::string("/tmp/os_op_test_") + std::to_string(std::time(0));

     // 使用echo命令创建一个随机名称的文件并写入两行内容
     std::string cmd_touch = std::string("echo \"hello\nworld\" >> ") + filename;
     std::vector<std::string> output_touch;
     succ = chef::os_exec_op::run_command(cmd_touch, &output_touch, &exit_status);
     assert(succ && output_touch.empty() && exit_status == 0);

     // 使用cat命令获取文件内容
     std::string cmd_cat = "cat " + filename;
     std::vector<std::string> output_cat;
     succ = chef::os_exec_op::run_command(cmd_cat, &output_cat, &exit_status);
     assert(succ && output_cat.size() == 2 && output_cat[0] == "hello" && output_cat[1] == "world" && exit_status == 0);

     // 使用rm命令删除文件
     std::string cmd_rm = "rm " + filename;
     std::vector<std::string> output_rm;
     succ = chef::os_exec_op::run_command(cmd_rm, &output_rm, &exit_status);
     assert(succ && output_rm.empty() && exit_status == 0);

     // 测试不存在的命令
     std::string cmd_not_exist = std::string("/tmp/cmd_not_exist_") + std::to_string(std::time(0));
     std::vector<std::string> output_cmd_not_exist;
     succ = chef::os_exec_op::run_command(cmd_not_exist, &output_cmd_not_exist, &exit_status);
     assert(succ && output_cmd_not_exist.empty() && exit_status != 0);

     // 测试删除不存在的文件
     std::string cmd_rm_not_exist = std::string("rm /tmp/file_not_exist_") + std::to_string(std::time(0));
     std::vector<std::string> output_cmd_rm_not_exist;
     succ = chef::os_exec_op::run_command(cmd_rm_not_exist, &output_cmd_rm_not_exist, &exit_status);
     assert(succ && output_cmd_not_exist.empty() && exit_status != 0);
     ```
 *
 */

#ifndef _CHEF_BASE_OS_EXEC_OP_HPP_
#define _CHEF_BASE_OS_EXEC_OP_HPP_
#pragma once

#include <string>
#include <vector>

namespace chef {

  class os_exec_op {
    public:
      /**
       * 开启子进程执行shell命令，并阻塞式等待结果
       *
       * @param cmd 需执行的命令
       * @param[output] output_lines 命令执行完后的标准输出结果，以行为单位存储
       * @param[output] exit_status 命令执行完后的退出状态码
       *
       * @return 命令执行返回true，错误返回false
       */
      static bool run_command(const std::string &cmd, std::vector<std::string> *output_lines, int *exit_status);

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

  inline bool os_exec_op::run_command(const std::string &cmd, std::vector<std::string> *output_lines, int *exit_status) {
    FILE *fp = popen(cmd.c_str(), "r");
    if (!fp) { return false; }

    static const int MAX_LEN = 16384;
    char line[MAX_LEN] = {0};
    for (; fgets(line, MAX_LEN, fp); ) {
      size_t rend = strlen(line) - 1;
      if (line[rend] == '\n') { line[rend] = '\0'; }

      if (output_lines) { output_lines->push_back(line); }
    }
    int es = pclose(fp);
    if (es == -1) { return false; }

    if (exit_status) { *exit_status = es; }

    return true;
  }

} // namespace chef

#endif // _CHEF_BASE_OS_EXEC_OP_HPP_
