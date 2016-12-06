#include "strings_op.h"
#include <sstream>
#include <cctype>
#include <stdint.h>

namespace chef {

  char strings_op::SPC = ' ';
  char strings_op::TAB = '\t';
  char strings_op::CR  = '\r';
  char strings_op::LF  = '\n';
  std::string strings_op::DIGITS          = "0123456789";
  std::string strings_op::OCTDIGITS       = "01234567";
  std::string strings_op::HEXDIGITS       = "0123456789abcdefABCDEF";
  std::string strings_op::LETTERS         = "abcdefghijklmnopqrstuvwxyzABCDEFGHIJKLMNOPQRSTUVWXYZ";
  std::string strings_op::LOWERCASE       = "abcdefghijklmnopqrstuvwxyz";
  std::string strings_op::UPPERCASE       = "ABCDEFGHIJKLMNOPQRSTUVWXYZ";
  std::string strings_op::PUNCTUATION     = "!\"#$%&\'()*+,-./:;<=>?@[\\]^_`{|}~";
  std::string strings_op::WRITESPACE      = " \t\n\r\x0b\x0c";
  std::string strings_op::PRINTABLE       = "0123456789abcdefghijklmnopqrstuvwxyzABCDEFGHIJKLMNOPQRSTUVWXYZ!\"#$%&\'()*+,-./:;<=>?@[\\]^_`{|}~ \t\n\r\x0b\x0c";

  int strings_op::compare(const std::string &a, const std::string &b) {
    return a.compare(b);
  }

  bool strings_op::contains(const std::string &s, const std::string &substr) {
    return s.find(substr) != std::string::npos;
  }

  bool strings_op::contains_any(const std::string &s, const std::string &charlist) {
    return s.find_first_of(charlist) != std::string::npos;
  }

  bool strings_op::has_prefix(const std::string &s, const std::string &prefix) {
    return s.find(prefix) == 0;
  }

  bool strings_op::has_suffix(const std::string &s, const std::string &suffix) {
    std::size_t pos = s.find(suffix);
    if (pos == std::string::npos) {
      return false;
    }
    return (suffix.length() + pos) == s.length();
  }

  int strings_op::count(const std::string &s, const std::string &sep) {
    if (s == sep) {
      return 1;
    }
    if (s.length() == 0 || sep.length() == 0) {
      return 0;
    }
    int count = 0;
    std::size_t pos = 0;
    for (; ;) {
      std::size_t found = s.find(sep, pos);
      if (found == std::string::npos) {
        break;
      }
      count++;
      pos = found + sep.length();
      if (pos >= s.length()) {
        break;
      }
    }
    return count;
  }

  std::string strings_op::to_lower(const std::string &s) {
    std::string ret;
//    for (auto &c : s) {
    for (std::string::const_iterator c = s.begin(); c != s.end(); c++) {
      ret += static_cast<char>(std::tolower(*c));
    }
    return ret;
  }

  std::string strings_op::to_upper(const std::string &s) {
    std::string ret;
//    for (auto &c : s) {
    for (std::string::const_iterator c = s.begin(); c != s.end(); c++) {
      ret += static_cast<char>(std::toupper(*c));
    }
    return ret;
  }

  std::vector<std::string> strings_op::split(const std::string &s, char sep, bool keep_empty_strings) {
    std::vector<std::string> ret;
    std::stringstream ss(s);
    std::string item;
    while(std::getline(ss, item, sep)) {
      if (!keep_empty_strings && item.empty()) {
        continue;
      }
      ret.push_back(item);
    }

    /// 如果最后一个字符是分隔符，那么后面需不需要append一个空item呢？
    /// std::getline的行为是不append，我认为要，所以处理一下~
    if (keep_empty_strings && *s.rbegin() == sep) {
      ret.push_back(std::string());
    }
    return ret;
  }

  std::vector<std::string> strings_op::splitlines(const std::string &s, bool keep_ends) {
    std::vector<std::string> ret;
    if (s.empty()) {
      return ret;
    }
    std::size_t len = s.length();
    std::size_t l = 0;
    std::size_t r = 0;
    std::string item;
    for (; r != len; r++) {
      if (s[r] == '\r') {
        item = s.substr(l, r-l);
        if (keep_ends) { item += '\r'; }
        if ((r+1) != len && s[r+1] == '\n') {
          r++;
          if (keep_ends) { item += '\n'; }
        }
        l = r+1;
        ret.push_back(item);
      } else if (s[r] == '\n') {
        item = s.substr(l, r-l);
        if (keep_ends) { item += '\n'; }
        l = r+1;
        ret.push_back(item);
      }
    }
    if (l < r) {
        ret.push_back(s.substr(l, r-l));
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
