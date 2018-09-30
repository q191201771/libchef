#include "../chef_base/chef_thread_group.hpp"
#include "../chef_base/chef_count_dump.hpp"
#include <stdio.h>
#include <unistd.h>
#include <pthread.h>
#include <sys/types.h>
#include <sys/syscall.h>
#include <iostream>
#include <sstream>
#include <atomic>
#include <mutex>
#include "./common/assert_wrapper.hpp"
#include "./common/check_log.hpp"

  inline uint64_t now_() {
    struct timespec ts;
#if defined(CLOCK_REALTIME) && !defined(__MACH__)
    clock_gettime(CLOCK_MONOTONIC, &ts);
#else
    {
      struct timeval tv;
      gettimeofday(&tv, NULL);
      ts.tv_sec = tv.tv_sec;
      ts.tv_nsec = tv.tv_usec * 1000;
    }
#endif
    return ts.tv_sec * 1000 + ts.tv_nsec / 1000000;
  }


static inline int _threadid() {
#if defined(__linux__)
  return static_cast<int>(syscall(SYS_gettid));
#elif defined(__MACH__)
  uint64_t tid64;
  pthread_threadid_np(NULL, &tid64);
  return static_cast<int>(tid64);
#endif
  return 0;
}

std::mutex m;

chef::multi_tag_counter mtc(chef::multi_tag_counter::MULTI_TAG_COUNTER_MUTEX);
chef::multi_tag_count_dumper mtcd(&mtc, 1000, 8, "chef_thread_group_test.dump");

void func(int i, int index) {
  (void)i;
  (void)index;
  //int sum = 0;
  //for (int j = 1; j < 1000; j++) {
  //  sum += j;
  //  for (int k = 1; k < 1000; k++) {
  //    sum += k;
  //  }
  //}
  //usleep(1000 * 1000);
  //std::lock_guard<std::mutex> guard(m);
  //std::cout << "i:" << i << ",index:"<< index << ",tid:" << _threadid() << ",sum:" << sum << std::endl;
  std::ostringstream oss;
  oss << _threadid();
  mtc.increment(oss.str());
}

static void init_test() {
  {
    chef::thread_group tg(8, chef::thread_group::INNER_DISPATCH_STRATEGY_RANDOM, "test");
  }
  {
    chef::thread_group tg(8, chef::thread_group::INNER_DISPATCH_STRATEGY_RANDOM, "test");
    tg.start();
    assert(tg.num_of_undone_task() == 0);
  }
}

static void add_with_index_test() {
  //std::cout << "> add_with_index_test.----------" << std::endl;
  chef::thread_group tg(8, chef::thread_group::INNER_DISPATCH_STRATEGY_RANDOM, "test");
  tg.start();
  int i = 0;
  tg.add(std::bind(&func, i++, 0), 0);
  tg.add(std::bind(&func, i++, 0), 0);
  tg.add(std::bind(&func, i++, 0), 0);
  tg.add(std::bind(&func, i++, 1), 1);
  tg.add(std::bind(&func, i++, 1), 1);
  tg.add(std::bind(&func, i++, 2), 2);
  tg.add(std::bind(&func, i++, 8), 8);
  tg.add(std::bind(&func, i++, 9), 9);

  while(tg.num_of_undone_task() != 0) {
    std::lock_guard<std::mutex> guard(m);
    //std::cout << "waiting." << std::endl;
  }
}

static void strategy_test(chef::thread_group::inner_dispatch_strategy strategy) {
  //std::cout << "> " << __func__ << ", " << strategy << std::endl;
  chef::thread_group tg(8, strategy, "test");
  tg.start();
  int i = 0;
  tg.add(std::bind(&func, i++, 0), 0);
  tg.add(std::bind(&func, i++, 1), 1);
  tg.add(std::bind(&func, i++, 2), 2);
  tg.add(std::bind(&func, i++, 3), 3);
  tg.add(std::bind(&func, i++, 4), 4);
  tg.add(std::bind(&func, i++, 5), 5);
  tg.add(std::bind(&func, i++, 6), 6);
  tg.add(std::bind(&func, i++, 7), 7);
  while(tg.num_of_undone_task() != 0) {
    std::lock_guard<std::mutex> guard(m);
    //std::cout << "waiting." << std::endl;
  }
  {
    std::lock_guard<std::mutex> guard(m);
    //std::cout << "-----" << std::endl;
  }

  //uint64_t b = now_();
  for (uint32_t j = 0; j < 10000; j++) {
    tg.add(std::bind(&func, j, 0));
  }

  while(tg.num_of_undone_task() != 0) {
    std::lock_guard<std::mutex> guard(m);
    //std::cout << "waiting." << std::endl;
  }
}

int main() {
  ENTER_TEST;

  mtcd.start();

  init_test();
  add_with_index_test();
  //strategy_test(chef::thread_group::INNER_DISPATCH_STRATEGY_ROUND_ROBIN);
  //strategy_test(chef::thread_group::INNER_DISPATCH_STRATEGY_RANDOM);
  strategy_test(chef::thread_group::INNER_DISPATCH_STRATEGY_IDLE);

  return 0;
}
