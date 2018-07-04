#include "../base/chef_thread_pool.hpp"
#include <stdio.h>
#include <unistd.h>
#include <atomic>
#include "./common/assert_wrapper.hpp"
#include "./common/check_log.hpp"

std::atomic<int> count(0);

void inc_count() {
  count++;
}

void inc_count_with_sleep() {
  count++;
  sleep(1);
}

void add_before_start_test() {
  count = 0;
  chef::thread_pool tp(2);
  for (int i = 0; i < 1024; i++) {
    tp.add(&inc_count);
  }
  assert(tp.num_of_undone_task() == 1024);
  tp.start();
  sleep(1);
  assert(count == 1024);
}

void normal_test(int num_of_thread) {
  count = 0;
  chef::thread_pool tp(num_of_thread);
  tp.start();
  for (int i = 0; i < 1024; i++) {
    tp.add(&inc_count);
  }
  sleep(1);
  assert(count == 1024);
}

void dctor_test() {
  {
    count = 0;
    chef::thread_pool tp(8);
    tp.start();
    for (int i = 0; i < 1024 * 1024; i++) {
      tp.add(&inc_count_with_sleep);
    }
  }
  assert(count < 1024 * 1024);
}

int main() {
  ENTER_TEST;

  add_before_start_test();
  normal_test(1);
  normal_test(16);
  dctor_test();

  return 0;
}
