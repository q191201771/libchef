/**
 * @tag      v1.5.16
 * @file     chef_strings_op.hpp
 * @deps     nope
 * @platform linux | macos | windows
 *
 * @author
 *   chef <191201771@qq.com>
 *     - initial release xxxx-xx-xx
 *
 * @brief    std::string常用操作帮助函数集合
 *
 */

#ifndef _CHEF_BASE_STRINGS_HPP_
#define _CHEF_BASE_STRINGS_HPP_
#pragma once

#include <sstream>
#include <string>
#include <vector>

namespace chef {

  static const char SPC = ' ';
  static const char TAB = '\t';
  static const char CR  = '\r';
  static const char LF  = '\n';
  static const std::string DIGITS      = "0123456789";
  static const std::string OCTDIGITS   = "01234567";
  static const std::string HEXDIGITS   = "0123456789abcdefABCDEF";
  static const std::string LETTERS     = "abcdefghijklmnopqrstuvwxyzABCDEFGHIJKLMNOPQRSTUVWXYZ";
  static const std::string LOWERCASE   = "abcdefghijklmnopqrstuvwxyz";
  static const std::string UPPERCASE   = "ABCDEFGHIJKLMNOPQRSTUVWXYZ";
  static const std::string PUNCTUATION = "!\"#$%&\'()*+,-./:;<=>?@[\\]^_`{|}~";
  static const std::string WHITESPACE  = " \t\n\r\x0b\x0c";
  static const std::string PRINTABLE   = "0123456789abcdefghijklmnopqrstuvwxyzABCDEFGHIJKLMNOPQRSTUVWXYZ!\"#$%&\'()*+,-./:;<=>?@[\\]^_`{|}~ \t\n\r\x0b\x0c";

  class strings_op {
    public:
      /// @return   0 或 >0 或 <0
      static int compare(const std::string &a, const std::string &b);

      /// `s`是否包含子串`target`
      static bool contains(const std::string &s, const std::string &target);

      /// `s`中是否包含`charlist`中的一个或多个字符
      static bool contains_any(const std::string &s, const std::string &charlist);

      /// `s`中是否包含`charlist`中的所有字符
      static bool contains_all(const std::string &s, const std::string &charlist);

      /// `s`是否以`prefix`开头
      static bool has_prefix(const std::string &s, const std::string &prefix);

      /// `s`是否以`suffix`结束
      static bool has_suffix(const std::string &s, const std::string &suffix);

      /// `s`中包含多少个`target`
      static int count(const std::string &s, const std::string &target);

      /// 转小写
      static std::string to_lower(const std::string &s);

      /// 转大写
      static std::string to_upper(const std::string &s);

      /// 从`s`头部开始，移除`charlist`中的字符，直到遇到不属于`charlist`的字符停止
      static std::string trim_left(const std::string &s, const std::string &charlist=WHITESPACE);

      /// 从`s`尾部开始，移除`charlist`中的字符，直到遇到不属于`charlist`的字符停止
      static std::string trim_right(const std::string &s, const std::string &charlist=WHITESPACE);

      /// 如果`s`以`prefix`开头，则移除`prefix`
      static std::string trim_prefix(const std::string &s, const std::string &prefix);

      /// 如果`s`以`suffix`结束，则移除`suffix`
      static std::string trim_suffix(const std::string &s, const std::string &suffix);

      /// 把`s`中的`target`全部替换成`replacement`
      static std::string replace(const std::string &s, const std::string &target, const std::string &replacement);

      /// 把`s`中的第一个`target`替换成`replacement`
      static std::string replace_first(const std::string &s, const std::string &target, const std::string &replacement);

      /// 把`s`中的最后一个`target`替换成`replacement`
      static std::string replace_last(const std::string &s, const std::string &target, const std::string &replacement);

      /**
       * 用`sep`分割`s`
       *
       * @param keep_empty_strings 分割后的字符串数组，如果有元素为空，是否保留这个元素
       *
       * @return 分割后的字符串数组
       *
       * @NOTICE
       *   if keep_empty_strings:
       *     '-a-b' -> ['', 'a', 'b']
       *     'a-b-' -> ['a', 'b', '']
       *     'a--b' -> ['a', '', 'b']
       *
       *   更多case见strings_op_test.cc
       *
       */
      static std::vector<std::string> split(const std::string &s, char sep, bool keep_empty_strings=true);

      /**
       * 允许分隔符是字符串
       * like s='ab--cd' and sep='--' -> ['ab', 'cd']
       *
       */
      static std::vector<std::string> split(const std::string &s, const std::string &sep, bool keep_empty_strings=true);

      /**
       * 可指定多个分隔符，一次到位~
       * like s='a-b*c' and sep='-*' -> ['a', 'b', 'c']
       *
       */
      static std::vector<std::string> split_any(const std::string &s, const std::string &charlist, bool keep_empty_strings=true);

      /**
       * 用 '\r' 或 '\n' 或 '\r\n' 分割
       *
       * @param keep_ends 如果为true，则把该行的结束符（'\r'或'\n'或'\r\n'）跟在该行后
       *
       */
      static std::vector<std::string> splitlines(const std::string &s, bool keep_ends=false);

      /// 把字符串数组`ss`用连接符`sep`连接起来，返回连接后的字符串
      static std::string join(const std::vector<std::string> &ss, const std::string &sep);

      ///
      template <class T>
      static std::string to_string(const T &t) {
          std::ostringstream ss;
          ss << t;
          return ss.str();
      }

    private:
      strings_op();
      strings_op(const strings_op &);
      strings_op &operator=(const strings_op &);

  }; // class strings_op

} // namespace chef





///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// @NOTICE 该分隔线以上部分为该模块的接口，分割线以下部分为对应的实现





#include <set>
#include <cctype>
#include <sstream>
#include <stdint.h>
#include <stdio.h>
#include <string.h>

namespace chef {

  inline int strings_op::compare(const std::string &a, const std::string &b) {
    return a.compare(b);
  }

  inline bool strings_op::contains(const std::string &s, const std::string &target) {
    return s.find(target) != std::string::npos;
  }

  inline bool strings_op::contains_any(const std::string &s, const std::string &charlist) {
    return s.find_first_of(charlist) != std::string::npos;
  }

  inline bool strings_op::contains_all(const std::string &s, const std::string &charlist) {
    if (s.empty() || charlist.empty() || s.length() < charlist.length()) {
      return false;
    }
    std::set<char> hash;
    for (std::size_t j = 0; j < s.length(); j++) {
      hash.insert(s[j]);
    }
    for (std::size_t i = 0; i < charlist.length(); i++) {
      if (hash.find(charlist[i]) == hash.end()) {
        return false;
      }
    }
    return true;
  }

  inline bool strings_op::has_prefix(const std::string &s, const std::string &prefix) {
    //return s.find(prefix) == 0;

    if (s.empty() || prefix.empty()) {
      return false;
    }
    return s.length() >= prefix.length() && s.substr(0, prefix.length()) == prefix;
  }

  inline bool strings_op::has_suffix(const std::string &s, const std::string &suffix) {
    //std::size_t pos = s.find(suffix);
    //if (pos == std::string::npos) {
    //  return false;
    //}
    //return (suffix.length() + pos) == s.length();

    if (s.empty() || suffix.empty()) {
      return false;
    }
    return s.length() >= suffix.length() && s.substr(s.length()-suffix.length(), suffix.length()) == suffix;
  }

  inline int strings_op::count(const std::string &s, const std::string &target) {
    if (s == target) {
      return 1;
    }
    if (s.empty() || target.empty()) {
      return 0;
    }
    int count = 0;
    std::size_t pos = 0;
    for (; ;) {
      std::size_t found = s.find(target, pos);
      if (found == std::string::npos) {
        break;
      }
      count++;
      pos = found + target.length();
      if (pos >= s.length()) {
        break;
      }
    }
    return count;
  }

  inline std::string strings_op::to_lower(const std::string &s) {
    std::string ret;
    for (std::string::const_iterator c = s.begin(); c != s.end(); ++c) {
      ret += static_cast<char>(std::tolower(*c));
    }
    return ret;
  }

  inline std::string strings_op::to_upper(const std::string &s) {
    std::string ret;
    for (std::string::const_iterator c = s.begin(); c != s.end(); ++c) {
      ret += static_cast<char>(std::toupper(*c));
    }
    return ret;
  }

  inline std::string strings_op::replace(const std::string &s, const std::string &target, const std::string &replacement) {
    if (s.empty() || target.empty()) {
      return s;
    }
    std::string ret;
    const std::size_t target_len = target.length();
    std::size_t l = 0;
    std::size_t r = 0;
    for (; ; ) {
      r = s.find(target, l);
      if (r == std::string::npos) {
        break;
      }
      ret.append(s.substr(l, r-l));
      ret.append(replacement);
      l = r + target_len;
    }
    if (l < s.length()) {
      ret.append(s.substr(l));
    }
    return ret;
  }

  inline std::string strings_op::replace_first(const std::string &s, const std::string &target, const std::string &replacement) {
    if (s.empty() || target.empty()) { return s; }

    std::string ret = s;
    std::size_t pos = ret.find(target);
    if (pos == std::string::npos) { return ret; }

    return ret.replace(pos, target.length(), replacement.c_str());

  }

  inline std::string strings_op::replace_last(const std::string &s, const std::string &target, const std::string &replacement) {
    if (s.empty() || target.empty()) { return s; }

    std::string ret = s;
    std::size_t pos = ret.rfind(target);
    if (pos == std::string::npos) { return ret; }

    return ret.replace(pos, target.length(), replacement.c_str());
  }

  inline std::vector<std::string> strings_op::split_any(const std::string &s, const std::string &charlist, bool keep_empty_strings) {
    std::vector<std::string> ret;
    if (s.empty()) {
      return ret;
    }
    if (charlist.empty()) {
      ret.push_back(s);
      return ret;
    }

    const std::size_t s_len = s.length();
    std::size_t l = 0;
    std::size_t r = 0;
    for (; ; ) {
      r = s.find_first_of(charlist, l);
      if (r != std::string::npos) {
        if (l != r || keep_empty_strings) {
          ret.push_back(s.substr(l, r-l));
        }
        l = r + 1;
      } else {
        if (l < s_len || keep_empty_strings) {
          ret.push_back(s.substr(l, r-l));
        }
        break;
      }
    }
    return ret;
  }

  inline std::vector<std::string> strings_op::split(const std::string &s, const std::string &sep, bool keep_empty_strings) {
    std::vector<std::string> ret;
    if (s.empty()) {
      return ret;
    }
    if (sep.empty()) {
      ret.push_back(s);
      return ret;
    }

    const std::size_t s_len   = s.length();
    const std::size_t sep_len = sep.length();
    std::size_t l = 0;
    std::size_t r = 0;
    for (; ; ) {
      r = s.find(sep, l);
      if (r != std::string::npos) {
        if (l != r || keep_empty_strings) {
          ret.push_back(s.substr(l, r-l));
        }
        l = r + sep_len;
      } else {
        if (l < s_len || keep_empty_strings) {
          ret.push_back(s.substr(l, r-l));
        }
        break;
      }
    }
    return ret;
  }

  inline std::vector<std::string> strings_op::split(const std::string &s, char sep, bool keep_empty_strings) {
	if (s.empty()) {
	  return std::vector<std::string>();
	}
    std::vector<std::string> ret;
    std::stringstream ss(s);
    std::string item;
    for (; std::getline(ss, item, sep); ) {
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

  inline std::vector<std::string> strings_op::splitlines(const std::string &s, bool keep_ends) {
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

  inline std::string strings_op::join(const std::vector<std::string> &ss, const std::string &sep) {
    if (ss.empty()) {
      return std::string();
    } if (ss.size() == 1) {
      return ss[0];
    }

    std::string ret;
    std::size_t i = 0;
    std::size_t size = ss.size();
    for (; i < size - 1; i++) {
      ret += ss[i] + sep;
    }
    ret += ss[i];
    return ret;
  }

  inline std::string strings_op::trim_left(const std::string &s, const std::string &charlist) {
    if (s.empty() || charlist.empty()) {
      return s;
    }
    std::size_t pos = 0;
    std::size_t sl = s.length();
    for (; pos < sl; pos++) {
      if (charlist.find(s[pos]) == std::string::npos) {
        break;
      }
    }
    return s.substr(pos);
  }

  inline std::string strings_op::trim_right(const std::string &s, const std::string &charlist) {
    if (s.empty() || charlist.empty()) {
      return s;
    }
    std::size_t pos = s.length() - 1;
    for (; ; ) {
      if (charlist.find(s[pos]) == std::string::npos) {
        break;
      }
      if (pos == 0) {
        pos = -1;
        break;
      }
      pos--;
    }
    return s.substr(0, pos + 1);
  }

  inline std::string strings_op::trim_prefix(const std::string &s, const std::string &prefix) {
    return has_prefix(s, prefix) ? s.substr(prefix.length()) : s;
  }

  inline std::string strings_op::trim_suffix(const std::string &s, const std::string &suffix) {
    return has_suffix(s, suffix) ? s.substr(0, s.length() - suffix.length()) : s;
  }

} // namespace chef

#endif // _CHEF_BASE_STRINGS_HPP_
