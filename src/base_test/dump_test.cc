#include "../base/dump.h"
#include <stdio.h>
#include <thread>
#include <chrono>
#include "./common/assert_wrapper.hpp"
#include "./common/check_log.hpp"

std::vector<std::string> initial_tags = {
  "queen",
  "worker",
  "editor",
  "reader",
  "teacher",
};
chef::dump d;
bool exit_flag = false;

void thd_fun() {
  int index = 0;
  while(!exit_flag) {
    index++;index = index % initial_tags.size();
    d.increment(initial_tags[index]);
    std::this_thread::sleep_for(std::chrono::milliseconds(100));
  }
}

int main() {
  ENTER_TEST;

  d.init(std::string("dump_test.dump"), initial_tags);
  d.add("queen", 100);
  d.add("admin", 12345678);
  d.add("sender", 20);
  d.add("doctor", 3000);
  d.add("nagetive", -100);

  std::thread t(std::bind(&thd_fun));
  //std::this_thread::sleep_for(std::chrono::seconds(60));
  std::this_thread::sleep_for(std::chrono::seconds(2));
  exit_flag = true;
  t.join();

  return 0;
}
