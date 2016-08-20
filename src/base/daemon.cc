#include "daemon.h"
#include <fcntl.h>
#include <unistd.h>
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <signal.h>

/**
 * 判断pidfile中pid对应的进程是否存在
 *
 * @return 存在返回pidfile中的pid，不存在返回-1
 */
static int exist(const char *pidfile) {
  int fp = open(pidfile, O_RDONLY, 0);
  if (fp == -1) {
    return -1;
  }
  char buf[64] = {0};
  int ret = read(fp, buf, 64);
  if (ret == -1) {
    return -1;
  }
  printf("buf: %s\n", buf);
  close(fp);
  char *p = strchr(buf, '\n');
  if (p != NULL) {
    *p = '\0';
  }
  int pid = atoi(buf);
  printf("pid: %d\n", pid);
  if (pid <= 0 || kill(pid, 0) == -1) {
    return -1;
  }
  return pid;
}

/**
 * 将当前pid写入文件
 * @return 成功返回进程id，失败返回-1
 */
static int write(const char *pidfile) {
  int fp = open(pidfile, O_WRONLY | O_CREAT | O_TRUNC, 0600);
  if (fp == -1) {
    return -1;
  }
  if (lockf(fp, F_TLOCK, 0) < 0) {
    return -1;
  }
  char buf[64] = {0};
  int pid = (int)::getpid();
  sprintf(buf, "%d\n", pid);
  int len = strlen(buf);
  int written = write(fp, buf, len);
  close(fp);
  if (len != written) {
    return -1;
  }
  return pid;
}

namespace chef {

int daemon::start(const char *pidfile) {
  int pid = exist(pidfile);
  if (pid > 0) {
    printf("1\n");
    exit(1);
  }
  if (::daemon(1, 0) == -1) {
    printf("2\n");
    exit(1);
  }
  pid = write(pidfile);
  if (pid == -1) {
    printf("3\n");
    exit(1);
  }
  return pid;
}

} // namespace chef
