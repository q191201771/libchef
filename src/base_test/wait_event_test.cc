#include "../base/wait_event.h"
#include <thread>
#include <atomic>
#include <unistd.h>
#include "assert_wrapper.h"

void notify_before_wait_test() {
  chef::wait_event we;
  we.notify();
  we.wait();
}

void notify_before_wait_for_test() {
  chef::wait_event we;
  we.notify();
  we.wait_for(1000 * 1000);
}

void wait_for_timeout_test() {
  chef::wait_event we;
  assert(we.wait_for(100) == false);
}

std::atomic<int> count(0);
chef::wait_event gwe;

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
  printf("Check wait_event.\n");

  notify_before_wait_test();
  notify_before_wait_for_test();
  wait_for_timeout_test();
  notify_after_wait_test();

  printf("Check wait_event done.\n");
  return 0;
}
