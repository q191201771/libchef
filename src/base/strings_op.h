/**
 * @file   strings.h/strings.cc
 * @deps   nope
 *
 * @author
 *   chef <191201771@qq.com>
 *     -initial release 2016-08-31
 *
 * @brief  std::string常用操作帮助函数
 *
 */

#ifndef _CHEF_BASE_STRINGS_H_
#define _CHEF_BASE_STRINGS_H_

#include <sstream>
#include <string>
#include <vector>

namespace chef {

  class strings_op {
    public:
      static const char SPC;
      static const char TAB;
      static const char CR;
      static const char LF;
      static const std::string DIGITS;
      static const std::string HEXDIGITS;
      static const std::string OCTDIGITS;
      static const std::string LETTERS;
      static const std::string LOWERCASE;
      static const std::string UPPERCASE;
      static const std::string PUNCTUATION;
      static const std::string WHITESPACE;
      static const std::string PRINTABLE;

    public:
      /// @return   0 或 >0 或 <0
      static int compare(const std::string &a, const std::string &b);

      /// <s>是否包含子串<substr>
      static bool contains(const std::string &s, const std::string &key);

      /// <s>中是否包含<charlist>中的任一字符
      static bool contains_any(const std::string &s, const std::string &charlist);

      /// <s>是否以<prefix>开头
      static bool has_prefix(const std::string &s, const std::string &prefix);

      /// <s>是否以<suffix>结束
      static bool has_suffix(const std::string &s, const std::string &suffix);

      /// <s>中包含多少个<key>
      static int count(const std::string &s, const std::string &key);

      /// 转小写
      static std::string to_lower(const std::string &s);

      /// 转大写
      static std::string to_upper(const std::string &s);

      /// 从<s>头部开始，移除<charlist>中的字符，直到遇到不属于<charlist>的字符停止
      static std::string trim_left(const std::string &s, const std::string &charlist=WHITESPACE);

      /// 从<s>尾部开始，移除<charlist>中的字符，直到遇到不属于<charlist>的字符停止
      static std::string trim_right(const std::string &s, const std::string &charlist=WHITESPACE);

      /// 如果<s>以<prefix>开头，则移除<prefix>
      static std::string trim_prefix(const std::string &s, const std::string &prefix);

      /// 如果<s>以<suffix>结束，则移除<suffix>
      static std::string trim_suffix(const std::string &s, const std::string &suffix);

      /**
       * 用<sep>分割<s>
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
       * 用 '\r' 或 '\n' 或 '\r\n' 分割
       *
       * @param keepends 如果为true，则把该行的结束符（'\r'或'\n'或'\r\n'）跟在该行后
       *
       */
      static std::vector<std::string> splitlines(const std::string &s, bool keep_ends=false);

      /// 把字符串数组<ss>用连接符<sep>连接起来，返回连接后的字符串
      static std::string join(const std::vector<std::string> &ss, const std::string &sep);

      ///
      template <class T>
      static std::string to_string(const T &t) {
          std::stringstream ss;
          ss << t;
          return ss.str();
      }
  };

} // namespace chef

#endif // _CHEF_BASE_STRINGS_H_
