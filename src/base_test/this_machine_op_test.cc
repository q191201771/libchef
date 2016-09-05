#include "../base/this_machine_op.h"
#include <stdio.h>
#include <assert.h>

int main() {
  printf("Check this_machine_op.\n");

  int noacc               = chef::this_machine_op::num_of_available_cpu_cores();
  int mem_total_kb        = chef::this_machine_op::mem_total_kb();
  int mem_free_kb         = chef::this_machine_op::mem_free_kb();
  int mem_buffers_kb      = chef::this_machine_op::mem_buffers_kb();
  int mem_cached_kb       = chef::this_machine_op::mem_cached_kb();
  int mem_used_kb         = chef::this_machine_op::mem_used_kb();
  int boot_timestamp      = chef::this_machine_op::boot_timestamp();
  int up_duration_seconds = chef::this_machine_op::up_duration_seconds();

  printf("num_of_available_cpu_cores: %d\n", noacc);
  printf("mem_total_kb: %d\n", mem_total_kb);
  printf("mem_free_kb: %d\n", mem_free_kb);
  printf("mem_buffers_kb: %d\n", mem_buffers_kb);
  printf("mem_cached_kb: %d\n", mem_cached_kb);
  printf("mem_used_kb: %d\n", mem_used_kb);
  printf("boot_timestamp: %d\n", boot_timestamp);
  printf("up_duration_seconds: %d\n", up_duration_seconds);

  assert(noacc > 0);
  assert(mem_total_kb > 0);
  assert(mem_free_kb > 0);
  assert(mem_buffers_kb > 0);
  assert(mem_cached_kb > 0);
  assert(mem_used_kb > 0);
  assert(boot_timestamp > 0);
  assert(up_duration_seconds > 0);

  printf("Check this_machine_op done.\n");
  return 0;
}
