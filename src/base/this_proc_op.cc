#include "this_proc_op.h"
#include "filepath_op.h"
#include <ctime>
#include <cstdlib>
#include <sstream>
#include <pwd.h>
#include <sys/types.h>
#include <unistd.h>
#include <stdint.h>

namespace inner {

  static const int64_t BOOT_TIMESTAMP = std::time(NULL);

  class proc_stat {
    public:
      std::string string_dummy_;
      char        ch_dummy_;
      int32_t     int32_dummy_;
      int64_t     int64_dummy_;
      int32_t     virt_kb_;
      int32_t     res_kb_;

      void parse(const char *content, int64_t page_size) {
        (void)content;
        int64_t vsize;
        int64_t res;
        std::istringstream iss(content);
        iss >> int32_dummy_ >> string_dummy_ >> ch_dummy_ >> int32_dummy_ >> int32_dummy_
            >> int32_dummy_ >> int32_dummy_ >> int32_dummy_ >> int32_dummy_
            >> int64_dummy_ >> int64_dummy_ >> int64_dummy_ >> int64_dummy_
            >> int64_dummy_ >> int64_dummy_ >> int64_dummy_ >> int64_dummy_
            >> int64_dummy_ >> int64_dummy_ >> int64_dummy_ >> int64_dummy_ >> int64_dummy_
            >> vsize >> res >> int64_dummy_;
        virt_kb_ = static_cast<int32_t>(vsize / 1024);
        res_kb_ = static_cast<int32_t>(res * (page_size / 1024));
      }
  };

} // namespace inner

namespace chef {

  int32_t this_proc_op::pid() {
    return static_cast<int32_t>(::getpid());
  }

  int32_t this_proc_op::ppid() {
    return static_cast<int32_t>(::getppid());
  }

  int32_t this_proc_op::uid() {
    return static_cast<int32_t>(::getuid());
  }

  int32_t this_proc_op::euid() {
    return static_cast<int32_t>(::geteuid());
  }

  std::string this_proc_op::user_name() {
    int64_t bufsize = ::sysconf(_SC_GETPW_R_SIZE_MAX);
    if (bufsize == -1) {
      bufsize = 8192;
    }
    char *buf = new char[bufsize];
    struct passwd pwd;
    struct passwd *result = NULL;
    if (getpwuid_r(::getuid(), &pwd, buf, bufsize, &result) != 0 || result == NULL) {
      delete []buf;
      return std::string();
    }
    delete []buf;
    return pwd.pw_name;
  }

  int64_t this_proc_op::boot_timestamp() {
    return inner::BOOT_TIMESTAMP;
  }

  int64_t this_proc_op::up_duration_seconds() {
    return std::time(NULL) - inner::BOOT_TIMESTAMP;
  }

  int32_t this_proc_op::num_of_threads() {
    std::string status = this_proc_op::status();
    size_t pos = status.find("Threads:");
    if (pos == std::string::npos) {
      return 0;
    }
    return std::atoi(status.c_str() + pos + 8);
  }

  std::string this_proc_op::exe_filepath() {
    return chef::filepath_op::read_link("/proc/self/exe", 256);
  }

  std::string this_proc_op::exe_path() {
    std::string filepath = exe_filepath();
    if (filepath == std::string()) {
      return std::string();
    }
    std::size_t pos = filepath.find_last_of('/');
    if (pos == std::string::npos) {
      return std::string();
    }
    return std::string(filepath, 0, pos + 1);
  }

  std::string this_proc_op::exe_name() {
    std::string filepath = exe_filepath();
    if (filepath == std::string()) {
      return std::string();
    }
    std::size_t pos = filepath.find_last_of('/');
    if (pos == std::string::npos) {
      return std::string();
    }
    return std::string(filepath, pos + 1);
  }

  std::string this_proc_op::status() {
    return filepath_op::read_file("/proc/self/status", 65535);
  }

  std::string this_proc_op::stat() {
    return filepath_op::read_file("/proc/self/stat", 65535);
  }

  int64_t this_proc_op::page_size() {
    return ::sysconf(_SC_PAGE_SIZE);
  }

  int32_t this_proc_op::virt_kb() {
    inner::proc_stat ps;
    ps.parse(stat().c_str(), page_size());
    return ps.virt_kb_;
  }

  int32_t this_proc_op::res_kb() {
    inner::proc_stat ps;
    ps.parse(stat().c_str(), page_size());
    return ps.res_kb_;
  }

} // namespace chef
