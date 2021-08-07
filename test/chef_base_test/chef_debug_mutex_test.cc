#include "chef_base/chef_debug_mutex.hpp"
#include "./common/assert_wrapper.hpp"
#include "./common/check_log.hpp"

#include <unistd.h>

int main() {
  ENTER_TEST;

  // 修改一些配置
  chef::debug_mutex_config::instance().logfd = stdout;
  chef::debug_mutex_config::instance().print_interval = 100;
  chef::debug_mutex_config::instance().long_wait_acquire_duration = 200;
  chef::debug_mutex_config::instance().long_hold_duration = 300;

  chef::debug_mutex dma;
  dma.lock();
  dma.unlock();

  chef::debug_mutex dmb;
  chef::debug_lock_guard guarda(dmb);

  chef::debug_mutex dmc(__FILE__, __LINE__);
  dmc.lock(__FILE__, __LINE__);
  usleep(100 * 1000);
  dmc.unlock(__FILE__, __LINE__);

  chef::debug_mutex dmd(__FILE__, __LINE__);
  DEBUG_MUTEX_LOCK(dmd);
  usleep(200 * 1000);
  DEBUG_MUTEX_UNLOCK(dmd);

  chef::debug_mutex dme(__FILE__, __LINE__);
  chef::debug_lock_guard guarb(dme, __FILE__, __LINE__);

  usleep(500 * 1000);

  return 0;
}
