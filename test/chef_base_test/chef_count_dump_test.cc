#include "chef_base/chef_count_dump.hpp"
#include <stdio.h>
#include <thread>
#include <chrono>
#include "./common/assert_wrapper.hpp"
#include "./common/check_log.hpp"

void mtc_free_lock_or_mutex_test(chef::multi_tag_counter::multi_tag_counter_type type) {
  int64_t num = 0;
  std::map<std::string, int64_t> counters;

  chef::multi_tag_counter mtc(type);

  //chef::multi_tag_count_dumper mtcd(&mtc, 100, 8, "mtcd1.txt", chef::multi_tag_count_dumper::MULTI_TAG_COUNT_DUMP_TYPE_APPEND);
  //mtcd.start();

  for (int i = 0; i < 100; i++) {
    assert(mtc.increment("1st"));
  }
  for (int i = 0; i < 100; i++) {
    assert(mtc.add_count("2nd", i));
  }
  //sleep(1);
  assert(mtc.set_count("3rd", 101));
  assert(mtc.get_tag_count("1st", &num));
  assert(num==100);
  assert(mtc.get_tag_count("2nd", &num));
  assert(num==4950);
  assert(mtc.get_tag_count("3rd", &num));
  assert(num==101);

  counters = mtc.get_tags_count();
  assert(counters.size() == 3 && counters["1st"] == 100 && counters["2nd"] == 4950 && counters["3rd"] == 101);

}

void mtc_atomic_test() {
  int64_t num = 0;
  std::map<std::string, int64_t> counters;
  chef::multi_tag_counter mtc(chef::multi_tag_counter::MULTI_TAG_COUNTER_ATOMIC);
  mtc.add_tag("1st");
  mtc.add_tag("2nd");

  for (int i = 0; i < 100; i++) {
    assert(mtc.increment("1st"));
  }
  for (int i = 0; i < 100; i++) {
    assert(mtc.add_count("2nd", i));
  }
  assert(!mtc.set_count("3rd", 101));
  assert(mtc.get_tag_count("1st", &num));
  assert(num==100);
  assert(mtc.get_tag_count("2nd", &num));
  assert(num==4950);
  assert(!mtc.get_tag_count("3rd", &num));

  counters = mtc.get_tags_count();
  assert(counters.size() == 2 && counters["1st"] == 100 && counters["2nd"] == 4950);

  //chef::multi_tag_count_dumper mtcd(&mtc, 1000, 8, "mtcd2.txt");
  //mtcd.start();
}

void multi_tag_counter_test() {
  { // normal
    mtc_free_lock_or_mutex_test(chef::multi_tag_counter::MULTI_TAG_COUNTER_FREE_LOCK);
    mtc_free_lock_or_mutex_test(chef::multi_tag_counter::MULTI_TAG_COUNTER_MUTEX);
    mtc_atomic_test();
  }
  { // corner
  }
}

int main() {
  ENTER_TEST;

  //count_type_immutable_test();
  //count_type_mutable_test();
  multi_tag_counter_test();

  return 0;
}
