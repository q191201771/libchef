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

  static const int START_TIMESTAMP = (int)std::time(NULL);

  static std::string read_file(const std::string &filename, int content_size = 65535) {
    char *content = new char[content_size];
    int read_size = chef::filepath_op::read_file(filename.c_str(), content, content_size);
    if (read_size == -1) {
      delete []content;
      return std::string();
    }
    std::string ret(content, read_size);
    delete []content;
    return ret;
  }

  class proc_stat {
    public:
      std::string string_dummy_;
      char        ch_dummy_;
      int32_t     int32_dummy_;
      int64_t     int64_dummy_;
      int32_t     virt_kb_;
      int32_t     res_kb_;

      void parse(const char *content, int page_size) {
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
        virt_kb_ = vsize / 1024;
        res_kb_ = res * (page_size / 1024);
      }
  };

} // namespace inner

namespace chef {

  int this_proc_op::pid() {
    return (int)::getpid();
  }

  int this_proc_op::ppid() {
    return (int)::getppid();
  }

  int this_proc_op::uid() {
    return (int)::getuid();
  }

  int this_proc_op::euid() {
    return (int)::geteuid();
  }

  std::string this_proc_op::user_name() {
    int bufsize = ::sysconf(_SC_GETPW_R_SIZE_MAX);
    if (bufsize == -1) {
      bufsize = 8192;
    }
    char buf[bufsize] = {0};
    struct passwd pwd;
    struct passwd *result = NULL;
    if (getpwuid_r(::getuid(), &pwd, buf, bufsize, &result) != 0 || result == NULL) {
      return std::string();
    }
    return pwd.pw_name;
  }

  int this_proc_op::start_timestamp() {
    return inner::START_TIMESTAMP;
  }

  int this_proc_op::started_duration_seconds() {
    int now = (int)std::time(NULL);
    return now - inner::START_TIMESTAMP;
  }

  int this_proc_op::num_of_threads() {
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
    return inner::read_file("/proc/self/status", 65535);
  }

  std::string this_proc_op::stat() {
    return inner::read_file("/proc/self/stat", 65535);
  }

  int this_proc_op::page_size() {
    return ::sysconf(_SC_PAGE_SIZE);
  }

  int this_proc_op::virt_kb() {
    inner::proc_stat ps;
    ps.parse(stat().c_str(), page_size());
    return ps.virt_kb_;
  }

  int this_proc_op::res_kb() {
    inner::proc_stat ps;
    ps.parse(stat().c_str(), page_size());
    return ps.res_kb_;
  }

} // namespace chef
