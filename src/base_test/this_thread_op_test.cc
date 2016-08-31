#include "../base/this_thread_op.h"
#include <thread>
#include <iostream>
#include <stdio.h>
#include <assert.h>
#include <unistd.h>

int main() {
  printf("Check this_thread_op.\n");

  int tid = chef::this_thread_op::gettid();
  chef::this_thread_op::set_thread_name("hello");
  // std::cout << "std::this_thread::get_id()=" << std::this_thread::get_id() << std::endl;
  // std::cout << "chef::this_thread_op::gettid()=" << tid << std::endl;
  // sleep(1000 * 1000);
  (void)tid;

  printf("Check this_thread_op done.\n");
  return 0;
}
