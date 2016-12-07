#include "../base/count_dump.h"
#include <stdio.h>
#include <thread>
#include <chrono>
#include "./common/assert_wrapper.hpp"
#include "./common/check_log.hpp"

std::vector<std::string> INITIAL_TAGS = {
  "queen",
  "worker",
  "editor",
  "reader",
  "teacher",
};

chef::shared_ptr<chef::count_dump> mutable_cd   = chef::make_shared<chef::count_dump>();
chef::shared_ptr<chef::count_dump> immutable_cd = chef::make_shared<chef::count_dump>();
bool mutable_exit_flag   = false;
bool immutable_exit_flag = false;

void immutable_thd_fun() {
  uint64_t index = 0;
  while(!immutable_exit_flag) {
    index++;index = index % INITIAL_TAGS.size();
    immutable_cd->increment(INITIAL_TAGS[index]);
    std::this_thread::sleep_for(std::chrono::milliseconds(1));
  }
}

void count_type_immutable_test() {
  immutable_cd->init_with_constant_tags(std::string("immutable.dump"), INITIAL_TAGS);
  assert(immutable_cd->add("queen", 100) == 0);
  assert(immutable_cd->add("admin", 12345678) == -1);
  assert(immutable_cd->add("sender", 20) == -1);
  assert(immutable_cd->add("doctor", 3000) == -1);
  assert(immutable_cd->add("nagetive", -100) == -1);

  std::thread t(std::bind(&immutable_thd_fun));
  //std::this_thread::sleep_for(std::chrono::seconds(60));
  std::this_thread::sleep_for(std::chrono::seconds(2));
  immutable_exit_flag = true;
  t.join();
}

void count_type_mutable_test() {
  mutable_cd->init(std::string("mutable.dump"));
  assert(mutable_cd->add("queen", 100) == 0);
  assert(mutable_cd->add("admin", 12345678) == 0);
  assert(mutable_cd->add("sender", 20) == 0);
  assert(mutable_cd->add("doctor", 3000) == 0);
  assert(mutable_cd->add("nagetive", -100) == 0);
  assert(mutable_cd->add("new", -100) == 0);
  std::this_thread::sleep_for(std::chrono::seconds(2));
}

int main() {
  ENTER_TEST;

  count_type_immutable_test();
  count_type_mutable_test();

  return 0;
}
