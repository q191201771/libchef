/**
 * @tag      v1.5.13
 * @file     chef_daemon_op.hpp
 * @deps     nope
 * @platform linux | macos | xxx
 *
 * @author
 *   chef <191201771@qq.com>
 *     -initial release xxxx-xx-xx
 *
 * @brief    守护进程
 *
 */

#ifndef _CHEF_BASE_DAEMON_OP_HPP_
#define _CHEF_BASE_DAEMON_OP_HPP_
#pragma once

namespace chef {

  class daemon_op {
    public:
      /// 成功则返回新进程id，失败则直接退出进程(进程已存在或调用系统函数失败)
      static int start(const char *pidfile);

    private:
      /**
       * 判断`pidfile`中进程id对应的进程是否存活
       *
       * @return 进程存活返回进程id，进程不存在返回-1
       *
       */
      static int exist(const char *pidfile);

      /**
       * 将当前进程id写入文件（内部使用了文件锁，避免竞态写）
       *
       * @return 写入成功成功返回当前进程id，失败返回-1
       *
       */
      static int write(const char *pidfile);
  };

} // namespace chef





///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// @NOTICE 该分隔线以上部分为该模块的接口，分割线以下部分为对应的实现





#include <fcntl.h>
#include <unistd.h>
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <signal.h>

namespace chef {

  inline int daemon_op::exist(const char *pidfile) {
    int fp = open(pidfile, O_RDONLY, 0);
    if (fp == -1) { return -1; }

    char buf[64] = {0};
    ssize_t ret = read(fp, buf, 64);
    if (ret == -1) { return -1; }

    close(fp);
    char *p = strchr(buf, '\n');
    if (p != NULL) { *p = '\0'; }

    int pid = atoi(buf);
    if (pid <= 0 || kill(pid, 0) == -1) { return -1; }

    return pid;
  }

  inline int daemon_op::write(const char *pidfile) {
    int fp = open(pidfile, O_WRONLY | O_CREAT | O_TRUNC, 0600);
    if (fp == -1) { return -1; }

    if (lockf(fp, F_TLOCK, 0) < 0) { return -1; }

    char buf[64] = {0};
    pid_t pid = ::getpid();
    snprintf(buf, 64, "%d\n", pid);
    size_t len = strlen(buf);
    size_t written = ::write(fp, buf, len);
    close(fp);
    if (len != written) { return -1; }

    return pid;
  }

  inline int daemon_op::start(const char *pidfile) {
    int pid = exist(pidfile);
    if (pid > 0) { exit(1); }

    if (::daemon(1, 0) == -1) { exit(1); }

    pid = daemon_op::write(pidfile);
    if (pid == -1) { exit(1); }

    return pid;
  }

} // namespace chef


#endif // _CHEF_BASE_DAEMON_OP_H_
