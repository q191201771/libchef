#include "../base/stuff.h"
#include <assert.h>
#include <stdio.h>
#include <vector>

void readable_bytes_test() {
  assert(chef::stuff::readable_bytes(768) == "768.0B");
  assert(chef::stuff::readable_bytes(10000) == "9.8K");
  assert(chef::stuff::readable_bytes(100001221) == "95.4M");
  assert(chef::stuff::readable_bytes(1000) == "1000.0B");
  assert(chef::stuff::readable_bytes(1023) == "1023.0B");
  assert(chef::stuff::readable_bytes(1024) == "1.0K");
  assert(chef::stuff::readable_bytes(1025) == "1.0K");
  assert(chef::stuff::readable_bytes(1000UL * 1000) == "976.6K");
  assert(chef::stuff::readable_bytes(1024UL * 1024) == "1.0M");
  assert(chef::stuff::readable_bytes(1000UL * 1000 * 1000) == "953.7M");
  assert(chef::stuff::readable_bytes(1024UL * 1024 * 1024) == "1.0G");
  assert(chef::stuff::readable_bytes(1024UL * 1024 * 1024 + 1000UL * 1000 * 1000) == "1.9G");
  assert(chef::stuff::readable_bytes(1000UL * 1000 * 1000 * 1000) == "931.3G");
  assert(chef::stuff::readable_bytes(1024UL * 1024 * 1024 * 1024) == "1.0T");
  assert(chef::stuff::readable_bytes(1024UL * 1024 * 1024 * 1024 * 1024) == "1.0P");
  assert(chef::stuff::readable_bytes(1024UL * 1024 * 1024 * 1024 * 1024 * 1024) == "1.0E");
  assert(chef::stuff::readable_bytes(1024UL * 1024 * 1024 * 1024 * 1024 * 1024 * 15) == "15.0E");
  assert(chef::stuff::readable_bytes(0UL - 1) == "16.0E");
  assert(chef::stuff::readable_bytes(18446744073709551615UL) == "16.0E");
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
    std::string ip = chef::stuff::get_host_by_name(iter);
    (void)ip;
    //printf("%s: %s\n", iter.c_str(), ip.c_str());
  }
}

int main() {
  printf("Check stuff.\n");

  readable_bytes_test();
  get_host_by_name_test();

  printf("Check stuff done.\n");
  return 0;
}
