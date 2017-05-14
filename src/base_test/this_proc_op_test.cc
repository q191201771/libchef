#include "../base/this_proc_op.h"
#include <stdio.h>
#include <string.h>
#include <ctime>
#include <thread>
#include <chrono>
#include "./common/assert_wrapper.hpp"
#include "./common/check_log.hpp"

void boot_timestamp_test() {
  int64_t sts = chef::this_proc_op::boot_timestamp();
  std::this_thread::sleep_for(std::chrono::seconds(2));
  assert(sts < std::time(NULL));
  assert(sts == chef::this_proc_op::boot_timestamp());
  assert(chef::this_proc_op::up_duration_seconds() > 0);
}

void num_of_threads_test() {
  assert(chef::this_proc_op::num_of_threads() == 1);
  std::thread t([]() {
    std::this_thread::sleep_for(std::chrono::milliseconds(200));
  });
  assert(chef::this_proc_op::num_of_threads() == 2);
  t.join();
}

void memory_test() {
  int32_t res1 = chef::this_proc_op::res_kb();
  char *buf = new char[1024 * 1024 * 10];
  memset(buf, 'x', 1024 * 1024 * 10);
  int32_t res2 = chef::this_proc_op::res_kb();
  assert(res2 > res1);
  delete []buf;
}

int main() {
  ENTER_TEST;

  boot_timestamp_test();
  num_of_threads_test();
  memory_test();

  std::string version              = chef::this_proc_op::version();
  int32_t pid                      = chef::this_proc_op::pid();
  int32_t ppid                     = chef::this_proc_op::ppid();
  int32_t uid                      = chef::this_proc_op::uid();
  int32_t euid                     = chef::this_proc_op::euid();
  std::string user_name            = chef::this_proc_op::user_name();
  int64_t boot_timestamp           = chef::this_proc_op::boot_timestamp();
  int64_t up_duration_seconds      = chef::this_proc_op::up_duration_seconds();
  int32_t num_of_threads           = chef::this_proc_op::num_of_threads();
  std::string exe_filepath         = chef::this_proc_op::exe_filepath();
  std::string exe_path             = chef::this_proc_op::exe_path();
  std::string exe_name             = chef::this_proc_op::exe_name();
  int64_t page_size                = chef::this_proc_op::page_size();
  std::string status               = chef::this_proc_op::status();
  std::string stat                 = chef::this_proc_op::stat();
  int32_t virt_kb                  = chef::this_proc_op::virt_kb();
  int32_t res_kb                   = chef::this_proc_op::res_kb();

  printf("version: %s\n", version.c_str());
  printf("pid: %d\n", pid);
  printf("ppid: %d\n", ppid);
  printf("uid: %d\n", uid);
  printf("euid: %d\n", euid);
  printf("user_name: %s\n", user_name.c_str());
  printf("boot_timestamp: %ld\n", boot_timestamp);
  printf("up_duration_seconds: %ld\n", up_duration_seconds);
  printf("num_of_threads: %d\n", num_of_threads);
  printf("exe_filepath: %s\n", exe_filepath.c_str());
  printf("exe_path: %s\n", exe_path.c_str());
  printf("exe_name: %s\n", exe_name.c_str());
  printf("page_size: %ld\n", page_size);
  printf("virt_kb: %d\n", virt_kb);
  printf("res_kb: %d\n", res_kb);
  //printf("-----status-----\n%s\n----------", status.c_str());
  //printf("-----stat-----\n%s\n----------", stat.c_str());

  assert(version.length() == 22 || version == "unknown");
  assert(pid > 0);
  assert(ppid > 0);
  assert(uid > 0);
  assert(euid > 0);
  assert(!user_name.empty());
  assert(boot_timestamp > 0);
  assert(num_of_threads > 0);
  assert(!exe_filepath.empty());
  assert(!exe_path.empty());
  assert(!exe_name.empty());
  assert(page_size > 0);
  assert(!status.empty());
  assert(!stat.empty());
  assert(virt_kb > 0);
  assert(res_kb > 0);
  (void)up_duration_seconds;

  //std::this_thread::sleep_for(std::chrono::seconds(10000));

  return 0;
}
