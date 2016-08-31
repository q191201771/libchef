#include "strings.h"
#include <sstream>

namespace chef {

  int strings::compare(const std::string &a, const std::string &b) {
    return a.compare(b);
  }

  bool strings::contains(const std::string &s, const std::string &substr) {
    return s.find(substr) != std::string::npos;
  }

  bool strings::contains_any(const std::string &s, const std::string &chars) {
    for (auto &c : chars) {
      for (auto &sc : s) {
        if (c == sc) {
          return true;
        }
      }
    }
    return false;
  }

  bool strings::has_prefix(const std::string &s, const std::string &prefix) {
    return s.find(prefix) == 0;
  }

  bool strings::has_suffix(const std::string &s, const std::string &suffix) {
    size_t sl = s.length();
    size_t cl = suffix.length();
    if (sl < cl) {
      return false;
    }
    return s.substr(sl - cl) == suffix;
  }

  int strings::count(const std::string &s, const std::string &sep) {
    if (s == sep) {
      return 1;
    }
    if (s.length() == 0 || sep.length() == 0) {
      return 0;
    }
    int count = 0;
    int pos = 0;
    for (; ;) {
      auto found = s.find(sep, pos);
      if (found != std::string::npos) {
        count++;
        pos = found + sep.length();
        if (pos >= (int)s.length()) {
          break;
        }
      } else {
        break;
      }
    }
    return count;
  }

  std::string strings::to_lower(const std::string &s) {
    std::string ret;
    for (auto &c : s) {
      if (c >= 'A' && c <= 'Z') {
        ret += (c + 32);
      } else {
        ret += c;
      }
    }
    return ret;
  }

  std::string strings::to_upper(const std::string &s) {
    std::string ret;
    for (auto &c : s) {
      if (c >= 'a' && c <= 'z') {
        ret += (c - 32);
      } else {
        ret += c;
      }
    }
    return ret;
  }

  std::vector<std::string> strings::split(const std::string &s, char sep) {
    std::vector<std::string> ret;
    std::stringstream ss(s);
    std::string item;
    while(std::getline(ss, item, sep)) {
      ret.push_back(item);
    }
    return ret;
  }

  std::string strings::join(const std::vector<std::string> &ss, const std::string &sep) {
    if (ss.empty()) {
      return std::string();
    } if (ss.size() == 1) {
      return ss[0];
    }

    std::string ret;
    int i = 0;
    int size = ss.size();
    for (; i < size - 1; i++) {
      ret += ss[i] + sep;
    }
    ret += ss[i];
    return ret;
  }

  std::string strings::trim_left(const std::string &s, const std::string &cutset) {
    if (s.length() == 0 || cutset.length() == 0) {
      return s;
    }
    int pos = 0;
    int sl = s.length();
    for (; pos < sl; pos++) {
      if (cutset.find(s[pos]) == std::string::npos) {
        break;
      }
    }
    return s.substr(pos);
  }

  std::string strings::trim_right(const std::string &s, const std::string &cutset) {
    if (s.length() == 0 || cutset.length() == 0) {
      return s;
    }
    int pos = s.length() - 1;
    for (; pos >= 0; pos--) {
      if (cutset.find(s[pos]) == std::string::npos) {
        break;
      }
    }
    return s.substr(0, pos + 1);
  }

  std::string strings::trim_prefix(const std::string &s, const std::string &prefix) {
    return has_prefix(s, prefix) ? s.substr(prefix.length()) : s;
  }

  std::string strings::trim_suffix(const std::string &s, const std::string &suffix) {
    return has_suffix(s, suffix) ? s.substr(0, s.length() - suffix.length()) : s;
  }

} // namespace chef
