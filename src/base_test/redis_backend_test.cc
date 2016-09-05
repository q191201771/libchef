#include "../base/redis_backend.h"
#include <stdio.h>
#include "assert_wrapper.h"
#include <unistd.h>

void not_exist_redis_test() {
  chef::redis_backend rb("8.8.8.8", 8888, "rbt.nert", 1, 1);
  assert(rb.start() == -1);
  assert(rb.ping() == -1);
  std::vector<std::string> keys;
  std::vector<std::string> values;
  assert(rb.hgetall(keys, values) == -1);
}

void normal_test() {
  chef::redis_backend rb("127.0.0.1", 6379, "rbt.nt");
  assert(rb.start() == 0);
  assert(rb.ping() == 0);
  std::vector<std::string> keys;
  std::vector<std::string> values;
  assert(rb.hgetall(keys, values) == 0);
  assert(keys.size() == 0);
  assert(values.size() == 0);
  rb.async_hdel("1");
  rb.async_hset("1", "one");
  rb.async_hset("2", "two");
  rb.async_hset("3", "three");
  rb.async_hdel("2");
  sleep(1);
  assert(rb.hgetall(keys, values) == 0);
  assert(keys.size() == 2);
  assert(values.size() == 2);
  assert((keys[0] == "1" && keys[1] == "3" && values[0] == "one" && values[1] == "three") ||
    (keys[0] == "3" && keys[1] == "1" && values[0] == "three" && values[1] == "one"));
  rb.async_hdel("1");
  rb.async_hdel("2");
  rb.async_hdel("3");
  sleep(1);
}

void redis_break_test() {
  chef::redis_backend rb("127.0.0.1", 6379, "rbt.bt", 1, 1);
  assert(rb.start() == 0);
  assert(rb.ping() == 0);

  int dummy;

  /// 把redis-server关掉，再走下面的流程
  printf("shutdown redis-server,then hit 0+enter.\n");
  int ret = scanf("%d", &dummy);
  assert(rb.ping() == -1);
  rb.async_hset("1", "one");
  rb.async_hset("3", "three");
  printf("run up redis-server,then hit 0+enter.\n");
  ret = scanf("%d", &dummy);
  (void)dummy;
  (void)ret;
  sleep(2);
  std::vector<std::string> keys;
  std::vector<std::string> values;
  assert(rb.hgetall(keys, values) == 0);
  assert((keys[0] == "1" && keys[1] == "3" && values[0] == "one" && values[1] == "three") ||
    (keys[0] == "3" && keys[1] == "1" && values[0] == "three" && values[1] == "one"));
  rb.async_hdel("1");
  rb.async_hdel("3");
  sleep(1);
}

int main() {
  printf("Check redis_backend.\n");
  
  not_exist_redis_test();
  normal_test();
  redis_break_test();

  printf("Check redis_backend done.\n");
  return 0;
}
