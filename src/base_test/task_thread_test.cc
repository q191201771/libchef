#include "../base/chef_task_thread.h"
#include <stdio.h>
#include <unistd.h>
#include <atomic>
#include "./common/assert_wrapper.hpp"
#include "./common/check_log.hpp"

void empty_fun() {}

void ctor_test() {
  chef::task_thread tt;
  chef::task_thread tt2("ctor_test");
  assert(tt.num_of_undone_task() == 0);
  assert(tt.thread_name() == "task thread");
  assert(tt2.num_of_undone_task() == 0);
  assert(tt2.thread_name() == "ctor_test");
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

chef::wait_event_counter we;
std::atomic<int> count{0};

void parallel_helper() {
  if (++count == 4) {
    we.notify();
  }
}

void parallel_test() {
  chef::task_thread gtt;
  gtt.start();
  gtt.add(parallel_helper, 200);
  gtt.add(parallel_helper);
  gtt.add(parallel_helper, 100);
  gtt.add(parallel_helper);
  we.wait();
  assert(gtt.num_of_undone_task() == 0);
  assert(count == 4);
}

std::atomic<int> release_mode_count{0};

void release_mode_helper(int n) {
  release_mode_count = n;
}

void release_mode_test() {
  {
    release_mode_count = 0;
    chef::task_thread tt("RELEASE_MODE_ASAP", chef::task_thread::RELEASE_MODE_ASAP);
    tt.start();
    tt.add(std::bind(release_mode_helper, 1), 2000);
  }
  assert(release_mode_count.load() == 0);

  {
    release_mode_count = 0;
    chef::task_thread tt("RELEASE_MODE_DO_SHOULD_DONE", chef::task_thread::RELEASE_MODE_DO_SHOULD_DONE);
    tt.start();
    tt.add(std::bind(release_mode_helper, 1), 1);
    tt.add(std::bind(release_mode_helper, 10), 10);
    tt.add(std::bind(release_mode_helper, 2000), 2000);
    tt.add(std::bind(release_mode_helper, 2001), 2001);
    usleep(1000 * 100);
  }
  assert(release_mode_count.load() == 10);

  {
    release_mode_count = 0;
    chef::task_thread tt("RELEASE_MODE_DO_ALL_DONE", chef::task_thread::RELEASE_MODE_DO_ALL_DONE);
    tt.start();
    tt.add(std::bind(release_mode_helper, 1), 2000);
  }
  assert(release_mode_count.load() == 1);
}

class foo {
public:
  foo(int a) : a_(a) {}
  void set_a(int a) { a_ = a; }
  int a() const { return a_; }
private:
  int a_;
};

void bind_member_fun_test() {
  foo f(0);
  {
    chef::task_thread tt("bind_member_fun_test", chef::task_thread::RELEASE_MODE_DO_ALL_DONE);
    tt.start();
    tt.add(std::bind(&foo::set_a, &f, 1));
  }
  assert(f.a() == 1);
}

int main() {
  ENTER_TEST;

  ctor_test();
  start_and_add_test();
  add_and_start_test();
  parallel_test();
  release_mode_test();
  bind_member_fun_test();

  return 0;
}
