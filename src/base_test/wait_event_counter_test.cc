#include "../base/chef_wait_event_counter.h"
#include <thread>
#include <atomic>
#include <unistd.h>
#include "./common/assert_wrapper.hpp"
#include "./common/check_log.hpp"

void notify_before_wait_test() {
  chef::wait_event_counter wec;
  wec.notify();
  wec.wait();
}

std::atomic<int> count(0);
chef::wait_event_counter gwe;

void notify_after_wait_helper() {
  usleep(200);
  count = 1;
  gwe.notify();
}

void notify_after_wait_test() {
  std::thread thd(notify_after_wait_helper);
  count = 2;
  gwe.wait();
  assert(count = 1);
  thd.join();
}

void counter_test() {
  const int need_count = 16;
  chef::wait_event_counter wec(need_count);
  for (int i = 0; i < need_count; i++) {
    wec.notify();
  }
  wec.wait();
}

chef::wait_event_counter gwe2(4);

void counter_parallel_helper() {
  usleep(1000 * 200);
  gwe2.notify();
}

void counter_parallel_test() {
  std::thread thd1(counter_parallel_helper);
  std::thread thd2(counter_parallel_helper);
  gwe2.notify();
  gwe2.notify();
  gwe2.wait();
  thd1.join();
  thd2.join();
}

int main() {
  ENTER_TEST;

  notify_before_wait_test();
  notify_after_wait_test();
  counter_test();
  counter_parallel_test();

  return 0;
}
