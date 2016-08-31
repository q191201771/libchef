#include "stuff.h"
#include <unistd.h>
#include <sys/types.h>
#include <netdb.h>
#include <sstream>

namespace chef {

  int stuff::get_cpu_num() {
    return sysconf(_SC_NPROCESSORS_ONLN);
  }

  std::string stuff::get_exe_filepath() {
    char buf[512] = {0};
    ssize_t length = readlink("/proc/self/exe", buf, sizeof buf);
    if (length == -1) {
      return std::string();
    }
    return std::string(buf, length);
  }

  std::string stuff::get_exe_path() {
    std::string filepath = get_exe_filepath();
    if (filepath == std::string()) {
      return std::string();
    }
    std::size_t pos = filepath.find_last_of('/');
    if (pos == std::string::npos) {
      return std::string();
    }
    return std::string(filepath, 0, pos + 1);
  }

  std::string stuff::get_exe_name() {
    std::string filepath = get_exe_filepath();
    if (filepath == std::string()) {
      return std::string();
    }
    std::size_t pos = filepath.find_last_of('/');
    if (pos == std::string::npos) {
      return std::string();
    }
    return std::string(filepath, pos + 1);
  }

  std::string stuff::readable_bytes(uint64_t n) {
    char UNITS[] = {'B', 'K', 'M', 'G', 'T', 'P', 'E'}; // 'Z' 'Y'
    int index = 0;
    for (; n >> (index*10); index++) {
      if (index == 6) { /// can't do n >> 70
        ++index;
        break;
      }
    }
    index = index > 0 ? index - 1 : index;
    char buf[128] = {0};
    snprintf(buf, 127, "%.1f%c",
      float(n) / (index ? (1UL << (index*10)) : 1),
      UNITS[index]
    );
    return std::string(buf);
  }

  std::string stuff::get_host_by_name(const std::string &domain) {
    struct hostent *ht = ::gethostbyname(domain.c_str());
    if (ht == NULL || ht->h_length <= 0) {
      return std::string();
    }
    char result[64] = {0};
    snprintf(result, 63, "%hhu.%hhu.%hhu.%hhu",
      (uint8_t)ht->h_addr_list[0][0],
      (uint8_t)ht->h_addr_list[0][1],
      (uint8_t)ht->h_addr_list[0][2],
      (uint8_t)ht->h_addr_list[0][3]
    );
    return std::string(result);
  }

} // namespace chef
