#include "../base/chef_this_machine_op.hpp"
#include "../base/chef_stringify_stl.hpp"
#include <stdio.h>
#include "./common/assert_wrapper.hpp"
#include "./common/check_log.hpp"

int main() {
  ENTER_TEST;

  bool res;
  int32_t noacc               = chef::this_machine_op::num_of_available_cpu_cores();
  chef::this_machine_op::mem_info mi;
  res = chef::this_machine_op::obtain_mem_info(&mi);
  assert(res);
  int64_t boot_timestamp      = chef::this_machine_op::boot_timestamp();
  int64_t up_duration_seconds = chef::this_machine_op::up_duration_seconds();
  uint64_t in_bytes = 0;
  uint64_t out_bytes = 0;
  std::set<std::string> interfaces;
  res = chef::this_machine_op::net_interfaces(&interfaces);
  res = chef::this_machine_op::net_interface_bytes("notexist", &in_bytes, &out_bytes);
  assert(!res);
#ifdef __linux__
  res = chef::this_machine_op::net_interface_bytes("eth0", &in_bytes, &out_bytes);
#else
  res = chef::this_machine_op::net_interface_bytes("en0", &in_bytes, &out_bytes);
#endif
  assert(res);

  printf("num_of_available_cpu_cores: %d\n", noacc);
  printf("mem_total_kb: %d\n", mi.total_kb);
  printf("mem_free_kb: %d\n", mi.free_kb);
  printf("mem_buffers_kb: %d\n", mi.buffers_kb);
  printf("mem_cached_kb: %d\n", mi.cached_kb);
  printf("mem_used_kb: %d\n", mi.used_kb);
  printf("boot_timestamp: %lld\n", boot_timestamp);
  printf("up_duration_seconds: %lld\n", up_duration_seconds);
  printf("net interfaces: %s\n", chef::stringify_stl(interfaces).c_str());
  printf("in_bytes: %llu\n", in_bytes);
  printf("out_bytes: %llu\n", out_bytes);

  assert(noacc > 0);
  assert(mi.total_kb > 0);
  assert(mi.free_kb > 0);
#ifdef __linux__
  assert(mi.buffers_kb > 0);
  assert(mi.cached_kb > 0);
#endif
  assert(mi.used_kb > 0);
  assert(boot_timestamp > 0);
  assert(up_duration_seconds > 0);
  assert(in_bytes > 0);
  assert(out_bytes > 0);

  return 0;
}
