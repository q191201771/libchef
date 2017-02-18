#include "../base/chef_wait_event_counter.h"
#include <thread>
#include <atomic>
#include <unistd.h>
#include "./common/assert_wrapper.hpp"
#include "./common/check_log.hpp"

/// 因为chef_wait_event_counter能够提供chef_wait_event的功能，所谓就去掉了chef_wait_event
/// 保留这里单元测试，确保能无缝替换~

void wait_without_notify_test() {
  chef::wait_event_counter we;
  we.wait();
}

void notify_before_wait_test() {
  chef::wait_event_counter we;
  we.notify();
  we.wait();
}

void notify_before_wait_for_test() {
  chef::wait_event_counter we;
  we.notify();
  we.wait_for(1000 * 1000);
}

void wait_for_timeout_test() {
  chef::wait_event_counter we;
  assert(we.wait_for(100) == false);
}

std::atomic<int> count(0);
chef::wait_event_counter gwe;

void thd_fun() {
  usleep(200);
  count = 1;
  gwe.notify();
}

void notify_after_wait_test() {
  std::thread thd(thd_fun);
  count = 2;
  gwe.wait();
  assert(count = 1);
  thd.join();
}

int main() {
  ENTER_TEST;

  notify_before_wait_test();
  notify_before_wait_for_test();
  wait_for_timeout_test();
  notify_after_wait_test();
  // wait_without_notify_test();

  return 0;
}
