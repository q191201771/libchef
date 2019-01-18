/**
 * @license  this file is a part of libchef. more info see https://github.com/q191201771/libchef
 * @tag      v1.10.17
 * @file     chef_stringify_stl.hpp
 * @deps     nope
 * @platform linux/macos/xxx
 *
 * @author
 *   chef <191201771@qq.com>
 *     - initial release xxxx-xx-xx
 *
 * @brief    字符串化stl容器。支持所有stl类型容器，支持多维嵌套容器，支持容器元素为自定义类型，支持自定义样式
 *
     1. 支持所有stl类型容器，接口简单只有一个函数，业务方传入容器变量即可使用

       ```
       std::vector<int> vec({1,2,3});
       std::string str = chef::stringify_stl(vec);
       // "[1,2,3]"

       std::map<int, int> m;
       m[1] = 100; m[2] = 200; m[3] = 300;
       str = chef::stringify_stl(m);
       // "{1:100,2:200,3:300}"
       ```

    2. 支持多维嵌套容器

      ```
      std::vector<std::vector<int> > vec = {{1,2,3}, {4,5,6}};
      str = chef::stringify_stl(vec);
      // "[[1,2,3],[4,5,6]]"
      ```

    3. 支持容器元素为自定义类型

      ```
      struct SelfType {
        int a_;
        int b_;

        SelfType(int a, int b) : a_(a), b_(b) {  }
      };

      // @NOTICE 业务方自己实现自定义类型转换成string的函数
      std::string stringify_stl_one_(const SelfType &st) {
        std::ostringstream oss;
        oss << "(" << st.a_ << "," << st.b_ << ")";
        return oss.str();
      }

      std::vector<SelfType> vec;
      vec.push_back(SelfType(1,2));
      vec.push_back(SelfType(3,4));
      std::string str = chef::stringify_stl(vec);
      // "[(1,2),(3,4)]"
      ```

     4. 支持自定义样式

       ```
       // 自定义样式举例
       // chef::STRINGIFY_STL_STYLE_ONE_BEAUTY 是我提供的一个默认带缩进换行的样式，业务方可以自己构造一个
       // stringify_stl_one_style 类型的变量实现更高定制化的样式
       std::string str = chef::stringify_stl(vec, chef::STRINGIFY_STL_STYLE_ONE_BEAUTY);
       // [
       //   "1",
       //   "2",
       //   "3"
       // ]
       ```
 *
 */

#ifndef _CHEF_BASE_STRINGIFY_STL_HPP_
#define _CHEF_BASE_STRINGIFY_STL_HPP_
#pragma once

#include <string>
#include <sstream>
#include <array>
#include <vector>
#include <deque>
#include <forward_list>
#include <list>
#include <set>
#include <map>
#include <unordered_set>
#include <unordered_map>
#include <stack>
#include <queue>

namespace chef {

struct stringify_stl_one_style {
  std::string delimiter_whole_prefix_;
  std::string delimiter_item_prefix_;
  std::string delimiter_item_suffix_;
  std::string delimiter_item_sep_;
  std::string delimiter_whole_suffix_;

  stringify_stl_one_style(const std::string &delimiter_whole_prefix="[",
                          const std::string &delimiter_item_prefix="",
                          const std::string &delimiter_item_suffix="",
                          const std::string &delimiter_item_sep=",",
                          const std::string &delimiter_whole_suffix="]")
      : delimiter_whole_prefix_(delimiter_whole_prefix)
      , delimiter_item_prefix_(delimiter_item_prefix)
      , delimiter_item_suffix_(delimiter_item_suffix)
      , delimiter_item_sep_(delimiter_item_sep)
      , delimiter_whole_suffix_(delimiter_whole_suffix)
  {}
};

struct stringify_stl_two_style {
  std::string delimiter_whole_prefix_;
  std::string delimiter_key_prefix_;
  std::string delimiter_key_suffix_;
  std::string delimiter_kv_sep_;
  std::string delimiter_value_prefix_;
  std::string delimiter_value_suffix_;
  std::string delimiter_item_sep_;
  std::string delimiter_whole_suffix_;

  stringify_stl_two_style(const std::string &delimiter_whole_prefix="{",
                          const std::string &delimiter_key_prefix="",
                          const std::string &delimiter_key_suffix="",
                          const std::string &delimiter_kv_sep=":",
                          const std::string &delimiter_value_prefix="",
                          const std::string &delimiter_value_suffix="",
                          const std::string &delimiter_item_sep=",",
                          const std::string &delimiter_whole_suffix="}")
      : delimiter_whole_prefix_(delimiter_whole_prefix)
      , delimiter_key_prefix_(delimiter_key_prefix)
      , delimiter_key_suffix_(delimiter_key_suffix)
      , delimiter_kv_sep_(delimiter_kv_sep)
      , delimiter_value_prefix_(delimiter_value_prefix)
      , delimiter_value_suffix_(delimiter_value_suffix)
      , delimiter_item_sep_(delimiter_item_sep)
      , delimiter_whole_suffix_(delimiter_whole_suffix)
  {}
};

static const stringify_stl_one_style STRINGIFY_STL_STYLE_ONE_DEFAULT("[",      "",       "",      ",",      "]");
static const stringify_stl_one_style STRINGIFY_STL_STYLE_ONE_BEAUTY("[\n",    "  \"",   "\"",    ",\n",    "\n]");

static const stringify_stl_two_style STRINGIFY_STL_STYLE_TWO_DEFAULT("{",       "",        "",      ":",     "",    "",    ",",      "}");
static const stringify_stl_two_style STRINGIFY_STL_STYLE_TWO_BEAUTY("{\n",      "  \"",    "\"",    ": ",    "",    "",    ",\n",    "\n}");

std::string stringify_stl_one_(int val) { return std::to_string(val); }
std::string stringify_stl_one_(long val) { return std::to_string(val); }
std::string stringify_stl_one_(long long val) { return std::to_string(val); }
std::string stringify_stl_one_(unsigned val) { return std::to_string(val); }
std::string stringify_stl_one_(unsigned long val) { return std::to_string(val); }
std::string stringify_stl_one_(unsigned long long val)  { return std::to_string(val); }
std::string stringify_stl_one_(float val) { return std::to_string(val); }
std::string stringify_stl_one_(double val) { return std::to_string(val); }
std::string stringify_stl_one_(long double val) { return std::to_string(val); }
std::string stringify_stl_one_(const std::string &s) { return s; }

template <typename T>
std::string stringify_stl_one_(const T &val, const stringify_stl_one_style &style=STRINGIFY_STL_STYLE_ONE_DEFAULT) {
  if (val.empty()) { return std::string(); }

  std::ostringstream ss;
  ss << style.delimiter_whole_prefix_;
  typename T::const_iterator iter = val.begin();
  for (; ; ) {
    ss << style.delimiter_item_prefix_ << stringify_stl_one_(*iter++) << style.delimiter_item_suffix_;
    if (iter == val.end()) { break; }
    else { ss << style.delimiter_item_sep_; }

  }
  ss << style.delimiter_whole_suffix_;
  return ss.str();
}

template <typename T>
std::string stringify_stl_two_(const T &val, const stringify_stl_two_style &style=STRINGIFY_STL_STYLE_TWO_DEFAULT) {
  if (val.empty()) { return std::string(); }

  std::ostringstream ss;
  ss << style.delimiter_whole_prefix_;
  typename T::const_iterator iter = val.begin();
  for (; ; ) {
    ss << style.delimiter_key_prefix_ << stringify_stl_one_(iter->first) << style.delimiter_key_suffix_
       << style.delimiter_kv_sep_
       << style.delimiter_value_prefix_ << stringify_stl_one_(iter->second) << style.delimiter_value_suffix_;
    ++iter;
    if (iter == val.end()) { break; }
    else { ss << style.delimiter_item_sep_; }

  }
  ss << style.delimiter_whole_suffix_;
  return ss.str();
}

template <typename T, std::size_t N>
std::string stringify_stl(const std::array<T, N> &val,
                          const stringify_stl_one_style &style=STRINGIFY_STL_STYLE_ONE_DEFAULT
) { return stringify_stl_one_<std::array<T, N> >(val, style); }

template <typename T>
std::string stringify_stl(const std::vector<T> &val,
                          const stringify_stl_one_style &style=STRINGIFY_STL_STYLE_ONE_DEFAULT
) { return stringify_stl_one_<std::vector<T> >(val, style); }

template <typename T>
std::string stringify_stl(const std::deque<T> &val,
                          const stringify_stl_one_style &style=STRINGIFY_STL_STYLE_ONE_DEFAULT
) { return stringify_stl_one_<std::deque<T> >(val, style); }

template <typename T>
std::string stringify_stl(const std::forward_list<T> &val,
                          const stringify_stl_one_style &style=STRINGIFY_STL_STYLE_ONE_DEFAULT
) { return stringify_stl_one_<std::forward_list<T> >(val, style); }

template <typename T>
std::string stringify_stl(const std::list<T> &val,
                          const stringify_stl_one_style &style=STRINGIFY_STL_STYLE_ONE_DEFAULT
) { return stringify_stl_one_<std::list<T> >(val, style); }

template <typename T>
std::string stringify_stl(const std::set<T> &val,
                          const stringify_stl_one_style &style=STRINGIFY_STL_STYLE_ONE_DEFAULT
) { return stringify_stl_one_<std::set<T> >(val, style); }

template <typename T>
std::string stringify_stl(const std::multiset<T> &val,
                          const stringify_stl_one_style &style=STRINGIFY_STL_STYLE_ONE_DEFAULT
) { return stringify_stl_one_<std::multiset<T> >(val, style); }

template <typename T>
std::string stringify_stl(const std::unordered_set<T> &val,
                          const stringify_stl_one_style &style=STRINGIFY_STL_STYLE_ONE_DEFAULT
) { return stringify_stl_one_<std::unordered_set<T> >(val, style); }

template <typename T>
std::string stringify_stl(const std::unordered_multiset<T> &val,
                          const stringify_stl_one_style &style=STRINGIFY_STL_STYLE_ONE_DEFAULT
) { return stringify_stl_one_<std::unordered_multiset<T> >(val, style); }

template <typename K, typename T>
std::string stringify_stl(const std::map<K, T> &val,
                          const stringify_stl_two_style &style=STRINGIFY_STL_STYLE_TWO_DEFAULT
) { return stringify_stl_two_<std::map<K, T> >(val, style); }

template <typename K, typename T>
std::string stringify_stl(const std::multimap<K, T> &val,
                          const stringify_stl_two_style &style=STRINGIFY_STL_STYLE_TWO_DEFAULT
) { return stringify_stl_two_<std::multimap<K, T> >(val, style); }

template <typename K, typename T>
std::string stringify_stl(const std::unordered_map<K, T> &val,
                          const stringify_stl_two_style &style=STRINGIFY_STL_STYLE_TWO_DEFAULT
) { return stringify_stl_two_<std::unordered_map<K, T> >(val, style); }

template <typename K, typename T>
std::string stringify_stl(const std::unordered_multimap<K, T> &val,
                          const stringify_stl_two_style &style=STRINGIFY_STL_STYLE_TWO_DEFAULT
) { return stringify_stl_two_<std::unordered_multimap<K, T> >(val, style); }

// * deep copy noniterative containers e.g. stack,queue,priority_queue
// * string begin with stack and priority_queue's top and queue's front

#define CHEF_STRINGIFY_STL_NONITERATIVE(val, style, posfunc) \
  if (val.empty()) { return std::string(); } \
  std::ostringstream ss; \
  ss << style.delimiter_whole_prefix_; \
  for (; ; ) { \
    ss << style.delimiter_item_prefix_ << stringify_stl_one_(val.posfunc()) << style.delimiter_item_suffix_; \
    val.pop(); \
    if (val.empty()) { break; } \
    else { ss << style.delimiter_item_sep_; } \
  } \
  ss << style.delimiter_whole_suffix_; \
  return ss.str();


template <typename T>
std::string stringify_stl(std::stack<T> val,
                          const stringify_stl_one_style &style=STRINGIFY_STL_STYLE_ONE_DEFAULT
) { CHEF_STRINGIFY_STL_NONITERATIVE(val, style, top); }

template <typename T>
std::string stringify_stl(std::queue<T> val,
                          const stringify_stl_one_style &style=STRINGIFY_STL_STYLE_ONE_DEFAULT
) { CHEF_STRINGIFY_STL_NONITERATIVE(val, style, front); }

template <typename T>
std::string stringify_stl(std::priority_queue<T> val,
                          const stringify_stl_one_style &style=STRINGIFY_STL_STYLE_ONE_DEFAULT
) { CHEF_STRINGIFY_STL_NONITERATIVE(val, style, top); }


}; // namespace chef

#endif // _CHEF_BASE_STRINGIFY_STL_HPP_

