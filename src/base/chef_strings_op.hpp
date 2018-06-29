/**
 * @file     chef_strings_op.hpp[_impl]
 * @deps     nope
 * @platform linux/macos/windows
 *
 * @author
 *   chef <191201771@qq.com>
 *     -initial release xxxx-xx-xx
 *
 * @brief  std::string常用操作帮助函数
 *
 */

#ifndef _CHEF_BASE_STRINGS_H_
#define _CHEF_BASE_STRINGS_H_
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

      /// 允许分隔符是字符串
      /// like s='ab--cd' and sep='--' -> ['ab', 'cd']
      static std::vector<std::string> split(const std::string &s, const std::string &sep, bool keep_empty_strings=true);

      /// 可指定多个分隔符，一次到位~
      /// like s='a-b*c' and sep='-*' -> ['a', 'b', 'c']
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
  };

} // namespace chef

#include "chef_strings_op_impl.hpp"

#endif // _CHEF_BASE_STRINGS_H_
