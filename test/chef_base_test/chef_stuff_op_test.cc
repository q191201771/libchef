#include "chef_base/chef_stuff_op.hpp"
#include "chef_base/chef_strings_op.hpp"
#include "./common/assert_wrapper.hpp"
#include "./common/check_log.hpp"
#include <vector>

static void example() {

}

void readable_bytes_test() {
  assert(chef::stuff_op::readable_bytes(768) == "768.0B");
  assert(chef::stuff_op::readable_bytes(10000) == "9.8K");
  assert(chef::stuff_op::readable_bytes(100001221) == "95.4M");
  assert(chef::stuff_op::readable_bytes(1000) == "1000.0B");
  assert(chef::stuff_op::readable_bytes(1023) == "1023.0B");
  assert(chef::stuff_op::readable_bytes(1024) == "1.0K");
  assert(chef::stuff_op::readable_bytes(1025) == "1.0K");
  assert(chef::stuff_op::readable_bytes(1000UL * 1000) == "976.6K");
  assert(chef::stuff_op::readable_bytes(1024UL * 1024) == "1.0M");
  assert(chef::stuff_op::readable_bytes(1000UL * 1000 * 1000) == "953.7M");
  assert(chef::stuff_op::readable_bytes(1024UL * 1024 * 1024) == "1.0G");
  assert(chef::stuff_op::readable_bytes(1024UL * 1024 * 1024 + 1000UL * 1000 * 1000) == "1.9G");
  assert(chef::stuff_op::readable_bytes(1000UL * 1000 * 1000 * 1000) == "931.3G");
  assert(chef::stuff_op::readable_bytes(1024UL * 1024 * 1024 * 1024) == "1.0T");
  assert(chef::stuff_op::readable_bytes(1024UL * 1024 * 1024 * 1024 * 1024) == "1.0P");
  assert(chef::stuff_op::readable_bytes(1024UL * 1024 * 1024 * 1024 * 1024 * 1024) == "1.0E");
  assert(chef::stuff_op::readable_bytes(1024UL * 1024 * 1024 * 1024 * 1024 * 1024 * 15) == "15.0E");
  assert(chef::stuff_op::readable_bytes(0UL - 1) == "16.0E");
  assert(chef::stuff_op::readable_bytes(18446744073709551615UL) == "16.0E");
}

void get_host_by_name_test() {
  std::vector<std::string> domains = {
    "http://www.baidu.com/test?a=b",
    "http://www.baidu.com/test",
    "http://www.baidu.com/",
    "http://www.baidu.com",
    "www.baidu.com",
    "localhost",
    "not exist",
    "58.96.168.38"
  };
  for (auto iter : domains) {
    std::string ip = chef::stuff_op::get_host_by_name(iter.c_str());
    (void)ip;
    printf("%s: %s\n", iter.c_str(), ip.c_str());
  }
}


int main() {
  ENTER_TEST;

  example();

  std::cout << "unix timestamp msec:" << chef::stuff_op::unix_timestamp_msec() << std::endl;
  std::cout << "tick msec:" << chef::stuff_op::tick_msec() << std::endl;
  std::cout << "tid:" << chef::stuff_op::gettid() << std::endl;
  chef::stuff_op::set_thread_name("yokothd");

  readable_bytes_test();
  get_host_by_name_test();
  std::cout << "tick msec:" << chef::stuff_op::tick_msec() << std::endl;
  std::cout << "unix timestamp msec:" << chef::stuff_op::unix_timestamp_msec() << std::endl;

  char *buf;
  static const char BUF[] = "1234567890abcdefghijklmn";
  //buf = BUF;
  //buf = chef::PRINTABLE.c_str();
  uint8_t buf2[256];
  for (std::size_t i = 0; i < 255; i++) {
    buf2[i] = i;
  }
  buf = (char *)buf2;

  std::string hex;
  for (std::size_t i = 0; i < 2; i++) {
    //for (std::size_t j = strlen(buf)-1; j < strlen(buf); j++) {
    for (std::size_t j = 255; j < 256; j++) {
      std::cout << "-----\n";
      hex = chef::stuff_op::bytes_to_hex((const uint8_t *)buf, j+1, 16, i % 2);
      std::cout << hex;
    }
  }

  return 0;
}
