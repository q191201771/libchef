#include "../base/task_thread.h"
#include <stdio.h>
#include <atomic>
#include "./common/assert_wrapper.hpp"
#include "./common/check_log.hpp"

void empty_fun() {}

void ctor_test() {
  chef::task_thread tt;
  chef::task_thread tt2("ctor_test");
  assert(tt.num_of_undone_task() == 0);
  assert(tt2.num_of_undone_task() == 0);
}

void start_and_add_test() {
  chef::task_thread tt;
  tt.start();
  tt.add(&empty_fun);
}

void add_and_start_test() {
  chef::task_thread tt;
  tt.add(&empty_fun);
  assert(tt.num_of_undone_task() == 1);
  tt.start();
}

chef::task_thread gtt;
chef::wait_event we;
std::atomic<int> count{0};

void gfun() {
  if (++count == 4) {
    we.notify();
  }
}

void parallel_test() {
  gtt.start();
  gtt.add(gfun, 200);
  gtt.add(gfun);
  gtt.add(gfun, 100);
  gtt.add(gfun);
  we.wait();
  assert(gtt.num_of_undone_task() == 0);
  assert(count == 4);
}

int main() {
  ENTER_TEST;

  ctor_test();
  start_and_add_test();
  add_and_start_test();
  parallel_test();

  return 0;
}
