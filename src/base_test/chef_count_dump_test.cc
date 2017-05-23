#include "../base/chef_count_dump.h"
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
  for (; !immutable_exit_flag; ) {
    index++;index = index % INITIAL_TAGS.size();
    immutable_cd->increment(INITIAL_TAGS[index]);
    std::this_thread::sleep_for(std::chrono::milliseconds(1));
  }
}

void count_type_immutable_test() {
  immutable_cd->init_with_constant_tags(std::string("/tmp/immutable.dump"), INITIAL_TAGS);
  assert(immutable_cd->add("queen", 100) == 0);
  assert(immutable_cd->add("admin", 12345678) == -1);
  assert(immutable_cd->add("sender", 20) == -1);
  assert(immutable_cd->add("doctor", 3000) == -1);
  assert(immutable_cd->add("nagetive", -100) == -1);

  std::thread t(std::bind(&immutable_thd_fun));
  std::this_thread::sleep_for(std::chrono::seconds(1));
  immutable_exit_flag = true;
  t.join();
}

void count_type_mutable_test() {
  mutable_cd->init(std::string("/tmp/mutable.dump"));
  assert(mutable_cd->add("queen", 100) == 0);
  assert(mutable_cd->add("admin", 12345678) == 0);
  assert(mutable_cd->add("sender", 20) == 0);
  assert(mutable_cd->add("doctor", 3000) == 0);
  assert(mutable_cd->add("nagetive", -100) == 0);
  assert(mutable_cd->add("new", -100) == 0);

  std::map<std::string, int> kvs;
  kvs = mutable_cd->kvs();
  assert(kvs.size() == 6);
  assert(kvs["queen"] == 100 && kvs["admin"] == 12345678 && kvs["sender"] == 20 && kvs["doctor"] == 3000 &&
         kvs["nagetive"] == -100 && kvs["new"] == -100);
  printf("%s\n", mutable_cd->stringify().c_str());
  printf("%s\n", mutable_cd->styled_stringify().c_str());

  assert(mutable_cd->reset("key-not-exist") == -1);
  kvs = mutable_cd->kvs();
  assert(kvs.size() == 6);
  assert(kvs["queen"] == 100 && kvs["admin"] == 12345678 && kvs["sender"] == 20 && kvs["doctor"] == 3000 &&
         kvs["nagetive"] == -100 && kvs["new"] == -100);

  assert(mutable_cd->reset("queen") == 0);
  kvs = mutable_cd->kvs();
  assert(kvs.size() == 6);
  assert(kvs["queen"] == 0 && kvs["admin"] == 12345678 && kvs["sender"] == 20 && kvs["doctor"] == 3000 &&
         kvs["nagetive"] == -100 && kvs["new"] == -100);

  mutable_cd->reset();
  kvs = mutable_cd->kvs();
  assert(kvs.size() == 6);
  assert(kvs["queen"] == 0 && kvs["admin"] == 0 && kvs["sender"] == 0 && kvs["doctor"] == 0 &&
         kvs["nagetive"] == 0 && kvs["new"] == 0);
}

int main() {
  ENTER_TEST;

  count_type_immutable_test();
  count_type_mutable_test();

  return 0;
}
