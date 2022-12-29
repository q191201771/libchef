#include "./common/assert_wrapper.hpp"
#include "./common/check_log.hpp"
#include "chef_base/chef_sync_once.hpp"

int main() {
  ENTER_TEST;

  chef::sync_once once;
  std::atomic<int> count;
  for (int i = 0; i < 8; i++) {
    once.run([&]() {
      count++;
    });
  }
  assert(count == 1);
}
