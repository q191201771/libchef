#include "strings_op.h"
#include <sstream>
#include <cctype>
#include <stdint.h>

namespace chef {

  int strings_op::compare(const std::string &a, const std::string &b) {
    return a.compare(b);
  }

  bool strings_op::contains(const std::string &s, const std::string &substr) {
    return s.find(substr) != std::string::npos;
  }

  bool strings_op::contains_any(const std::string &s, const std::string &charlist) {
    for (auto &c : charlist) {
      for (auto &sc : s) {
        if (c == sc) {
          return true;
        }
      }
    }
    return false;
  }

  bool strings_op::has_prefix(const std::string &s, const std::string &prefix) {
    return s.find(prefix) == 0;
  }

  bool strings_op::has_suffix(const std::string &s, const std::string &suffix) {
    size_t sl = s.length();
    size_t cl = suffix.length();
    if (sl < cl) {
      return false;
    }
    return s.substr(sl - cl) == suffix;
  }

  int strings_op::count(const std::string &s, const std::string &sep) {
    if (s == sep) {
      return 1;
    }
    if (s.length() == 0 || sep.length() == 0) {
      return 0;
    }
    int count = 0;
    size_t pos = 0;
    for (; ;) {
      size_t found = s.find(sep, pos);
      if (found != std::string::npos) {
        count++;
        pos = found + sep.length();
        if (pos >= s.length()) {
          break;
        }
      } else {
        break;
      }
    }
    return count;
  }

  std::string strings_op::to_lower(const std::string &s) {
    std::string ret;
    for (auto &c : s) {
      ret += static_cast<char>(std::tolower(c));
    }
    return ret;
  }

  std::string strings_op::to_upper(const std::string &s) {
    std::string ret;
    for (auto &c : s) {
      ret += static_cast<char>(std::toupper(c));
    }
    return ret;
  }

  std::vector<std::string> strings_op::split(const std::string &s, char sep) {
    std::vector<std::string> ret;
    std::stringstream ss(s);
    std::string item;
    while(std::getline(ss, item, sep)) {
      ret.push_back(item);
    }
    return ret;
  }

  std::string strings_op::join(const std::vector<std::string> &ss, const std::string &sep) {
    if (ss.empty()) {
      return std::string();
    } if (ss.size() == 1) {
      return ss[0];
    }

    std::string ret;
    uint64_t i = 0;
    uint64_t size = ss.size();
    for (; i < size - 1; i++) {
      ret += ss[i] + sep;
    }
    ret += ss[i];
    return ret;
  }

  std::string strings_op::trim_left(const std::string &s, const std::string &charlist) {
    if (s.length() == 0 || charlist.length() == 0) {
      return s;
    }
    uint64_t pos = 0;
    uint64_t sl = s.length();
    for (; pos < sl; pos++) {
      if (charlist.find(s[pos]) == std::string::npos) {
        break;
      }
    }
    return s.substr(pos);
  }

  std::string strings_op::trim_right(const std::string &s, const std::string &charlist) {
    if (s.length() == 0 || charlist.length() == 0) {
      return s;
    }
    int64_t pos = s.length() - 1;
    for (; pos >= 0; pos--) {
      if (charlist.find(s[pos]) == std::string::npos) {
        break;
      }
    }
    return s.substr(0, pos + 1);
  }

  std::string strings_op::trim_prefix(const std::string &s, const std::string &prefix) {
    return has_prefix(s, prefix) ? s.substr(prefix.length()) : s;
  }

  std::string strings_op::trim_suffix(const std::string &s, const std::string &suffix) {
    return has_suffix(s, suffix) ? s.substr(0, s.length() - suffix.length()) : s;
  }

} // namespace chef
