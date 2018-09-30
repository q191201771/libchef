#include "../base/this_thread_op.h"
#include <thread>
#include <iostream>
#include <stdio.h>
#include <unistd.h>
#include "./common/assert_wrapper.hpp"
#include "./common/check_log.hpp"

int main() {
  ENTER_TEST;

  int tid = chef::this_thread_op::gettid();
  chef::this_thread_op::set_thread_name("hello");
  std::cout << "std::this_thread::get_id()=" << std::this_thread::get_id() << std::endl;
  std::cout << "chef::this_thread_op::gettid()=" << tid << std::endl;
  // sleep(1000 * 1000);
  (void)tid;

  return 0;
}
