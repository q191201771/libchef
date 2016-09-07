#include "../base/this_machine_op.h"
#include <stdio.h>
#include "./common/assert_wrapper.hpp"
#include "./common/check_log.hpp"

int main() {
  ENTER_TEST;

  int32_t noacc               = chef::this_machine_op::num_of_available_cpu_cores();
  int32_t mem_total_kb        = chef::this_machine_op::mem_total_kb();
  int32_t mem_free_kb         = chef::this_machine_op::mem_free_kb();
  int32_t mem_buffers_kb      = chef::this_machine_op::mem_buffers_kb();
  int32_t mem_cached_kb       = chef::this_machine_op::mem_cached_kb();
  int32_t mem_used_kb         = chef::this_machine_op::mem_used_kb();
  int64_t boot_timestamp      = chef::this_machine_op::boot_timestamp();
  int64_t up_duration_seconds = chef::this_machine_op::up_duration_seconds();
  uint64_t in_bytes;
  uint64_t out_bytes;
  assert(chef::this_machine_op::io_bytes("notexist", &in_bytes, &out_bytes) == -1);
  assert(chef::this_machine_op::io_bytes("enp0s3", &in_bytes, &out_bytes) == 0);

  printf("num_of_available_cpu_cores: %d\n", noacc);
  printf("mem_total_kb: %d\n", mem_total_kb);
  printf("mem_free_kb: %d\n", mem_free_kb);
  printf("mem_buffers_kb: %d\n", mem_buffers_kb);
  printf("mem_cached_kb: %d\n", mem_cached_kb);
  printf("mem_used_kb: %d\n", mem_used_kb);
  printf("boot_timestamp: %ld\n", boot_timestamp);
  printf("up_duration_seconds: %ld\n", up_duration_seconds);
  printf("in_bytes: %lu\n", in_bytes);
  printf("out_bytes: %lu\n", out_bytes);

  assert(noacc > 0);
  assert(mem_total_kb > 0);
  assert(mem_free_kb > 0);
  assert(mem_buffers_kb > 0);
  assert(mem_cached_kb > 0);
  assert(mem_used_kb > 0);
  assert(boot_timestamp > 0);
  assert(up_duration_seconds > 0);
  assert(in_bytes > 0);
  assert(out_bytes > 0);

  return 0;
}
