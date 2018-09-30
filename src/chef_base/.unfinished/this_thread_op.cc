#include "this_thread_op.h"
#include <sys/prctl.h>
#include <unistd.h>
#include <sys/syscall.h>

namespace chef {

  int this_thread_op::gettid() {
    static __thread int tid = 0;
    if (tid == 0) {
      tid = static_cast<int>(syscall(__NR_gettid));
    }
    return tid;
  }

  void this_thread_op::set_thread_name(const char *name) {
    ::prctl(PR_SET_NAME, name);
  }

} // namespace chef
