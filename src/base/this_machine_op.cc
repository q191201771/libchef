#include "this_machine_op.h"
#include "filepath_op.h"
#include <sys/sysinfo.h>
#include <ctime>
#include <cstdlib>
#include <cstring>

namespace inner {

  class meminfo {
    public:
      int mem_total_kb_;
      int mem_free_kb_;
      int mem_buffers_kb_;
      int mem_cached_kb_;
      int mem_used_kb_;

      void parse() {
        std::string content = chef::filepath_op::read_file("/proc/meminfo", 65535);
        mem_total_kb_   = parse_int(content, "MemTotal:");
        mem_free_kb_    = parse_int(content, "MemFree:");
        mem_buffers_kb_ = parse_int(content, "Buffers:");
        mem_cached_kb_  = parse_int(content, "Cached:");
        mem_used_kb_    = mem_total_kb_ - mem_free_kb_ - mem_buffers_kb_ - mem_cached_kb_;
      }
    private:
      int parse_int(const std::string &content, const std::string &key) {
        size_t pos = content.find(key);
        if (pos == std::string::npos) {
          return 0;
        }
        return std::atoi(content.c_str() + pos + key.length());
      }
  };

  class stat {
    public:
      stat() : btime_(0) {}

      int btime_;

      void parse() {
        std::string content = chef::filepath_op::read_file("/proc/stat", 65535);
        size_t pos = content.find("btime");
        if (pos == std::string::npos) {
          return;
        }
        btime_ = std::atoi(content.c_str() + pos + strlen("btime"));
      }
  };

} // namespace inner

namespace chef {

  int this_machine_op::num_of_available_cpu_cores() {
    return ::get_nprocs();
  }

  int this_machine_op::mem_total_kb() {
    inner::meminfo mi;
    mi.parse();
    return mi.mem_total_kb_;
  }

  int this_machine_op::mem_free_kb() {
    inner::meminfo mi;
    mi.parse();
    return mi.mem_free_kb_;
  }

  int this_machine_op::mem_buffers_kb() {
    inner::meminfo mi;
    mi.parse();
    return mi.mem_buffers_kb_;
  }

  int this_machine_op::mem_cached_kb() {
    inner::meminfo mi;
    mi.parse();
    return mi.mem_cached_kb_;
  }

  int this_machine_op::mem_used_kb() {
    inner::meminfo mi;
    mi.parse();
    return mi.mem_used_kb_;
  }

  int this_machine_op::boot_timestamp() {
    inner::stat st;
    st.parse();
    return st.btime_;
  }

  int this_machine_op::up_duration_seconds() {
    inner::stat st;
    st.parse();
    return (int)std::time(NULL) - st.btime_;
  }

} // namespace chef
