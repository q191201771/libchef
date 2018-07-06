/**
 * @file     chef_stringify_stl.hpp
 * @deps     nope
 * @platform linux/macos/xxx
 *
 * @author
 *   chef <191201771@qq.com>
 *     -initial release xxxx-xx-xx
 *
 * @brief    字符串化stl容器
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
//#include <stack>
//#include <queue>

// one 默认分隔    "["      ""       ""      ","      "]"
//     定制举例    "[\n"    "  \""   "\""    ",\n"    "\n]"
//
// two 默认分隔    "{"       ""        ""      ":"     ""    ""    ","      "}"
//     定制举例    "{\n}"    "  \""    "\""    ": "    ""    ""    ",\n"    "\n}"


namespace chef {

template <typename T>
std::string stringify_stl_one(const T &val
                              ,const std::string &delimiter_whole_prefix="["
                              ,const std::string &delimiter_item_prefix=""
                              ,const std::string &delimiter_item_suffix=""
                              ,const std::string &delimiter_item_sep=","
                              ,const std::string &delimiter_whole_suffix="]"
) {
  if (val.empty()) { return std::string(); }

  std::ostringstream ss;
  ss << delimiter_whole_prefix;
  typename T::const_iterator iter = val.begin();
  for (; ; ) {
    ss << delimiter_item_prefix << *iter++ << delimiter_item_suffix;
    if (iter == val.end()) { break; }
    else { ss << delimiter_item_sep; }

  }
  ss << delimiter_whole_suffix;
  return ss.str();
}

template <typename T>
std::string stringify_stl_two(const T &val
                             ,const std::string &delimiter_whole_prefix="{"
                             ,const std::string &delimiter_key_prefix=""
                             ,const std::string &delimiter_key_suffix=""
                             ,const std::string &delimiter_kv_sep=":"
                             ,const std::string &delimiter_value_prefix=""
                             ,const std::string &delimiter_value_suffix=""
                             ,const std::string &delimiter_item_sep=","
                             ,const std::string &delimiter_whole_suffix="}"
) {
  if (val.empty()) { return std::string(); }

  std::ostringstream ss;
  ss << delimiter_whole_prefix;
  typename T::const_iterator iter = val.begin();
  for (; ; ) {
    ss << delimiter_key_prefix << iter->first << delimiter_key_suffix
       << delimiter_kv_sep
       << delimiter_value_prefix << iter->second << delimiter_value_suffix;
    iter++;
    if (iter == val.end()) { break; }
    else { ss << delimiter_item_sep; }

  }
  ss << delimiter_whole_suffix;
  return ss.str();
}

template <typename T, std::size_t N>
std::string stringify_stl_array(const std::array<T, N> &val,
const std::string &delimiter_whole_prefix="[",const std::string &delimiter_item_prefix="",const std::string &delimiter_item_suffix="",const std::string &delimiter_item_sep=",",const std::string &delimiter_whole_suffix="]"
) { return stringify_stl_one<std::array<T, N> >(val,
delimiter_whole_prefix,delimiter_item_prefix,delimiter_item_suffix,delimiter_item_sep,delimiter_whole_suffix); }

template <typename T>
std::string stringify_stl_vector(const std::vector<T> &val,
const std::string &delimiter_whole_prefix="[",const std::string &delimiter_item_prefix="",const std::string &delimiter_item_suffix="",const std::string &delimiter_item_sep=",",const std::string &delimiter_whole_suffix="]"
) { return stringify_stl_one<std::vector<T> >(val,
delimiter_whole_prefix,delimiter_item_prefix,delimiter_item_suffix,delimiter_item_sep,delimiter_whole_suffix); }

template <typename T>
std::string stringify_stl_deque(const std::deque<T> &val,
const std::string &delimiter_whole_prefix="[",const std::string &delimiter_item_prefix="",const std::string &delimiter_item_suffix="",const std::string &delimiter_item_sep=",",const std::string &delimiter_whole_suffix="]"
    ) { return stringify_stl_one<std::deque<T> >(val,
delimiter_whole_prefix,delimiter_item_prefix,delimiter_item_suffix,delimiter_item_sep,delimiter_whole_suffix); }

template <typename T>
std::string stringify_stl_forward_list(const std::forward_list<T> &val,
const std::string &delimiter_whole_prefix="[",const std::string &delimiter_item_prefix="",const std::string &delimiter_item_suffix="",const std::string &delimiter_item_sep=",",const std::string &delimiter_whole_suffix="]"
    ) { return stringify_stl_one<std::forward_list<T> >(val,
delimiter_whole_prefix,delimiter_item_prefix,delimiter_item_suffix,delimiter_item_sep,delimiter_whole_suffix); }

template <typename T>
std::string stringify_stl_list(const std::list<T> &val,
const std::string &delimiter_whole_prefix="[",const std::string &delimiter_item_prefix="",const std::string &delimiter_item_suffix="",const std::string &delimiter_item_sep=",",const std::string &delimiter_whole_suffix="]"
    ) { return stringify_stl_one<std::list<T> >(val,
delimiter_whole_prefix,delimiter_item_prefix,delimiter_item_suffix,delimiter_item_sep,delimiter_whole_suffix); }

template <typename T>
std::string stringify_stl_set(const std::set<T> &val,
const std::string &delimiter_whole_prefix="[",const std::string &delimiter_item_prefix="",const std::string &delimiter_item_suffix="",const std::string &delimiter_item_sep=",",const std::string &delimiter_whole_suffix="]"
    ) { return stringify_stl_one<std::set<T> >(val,
delimiter_whole_prefix,delimiter_item_prefix,delimiter_item_suffix,delimiter_item_sep,delimiter_whole_suffix); }

template <typename T>
std::string stringify_stl_multiset(const std::multiset<T> &val,
const std::string &delimiter_whole_prefix="[",const std::string &delimiter_item_prefix="",const std::string &delimiter_item_suffix="",const std::string &delimiter_item_sep=",",const std::string &delimiter_whole_suffix="]"
    ) { return stringify_stl_one<std::multiset<T> >(val,
delimiter_whole_prefix,delimiter_item_prefix,delimiter_item_suffix,delimiter_item_sep,delimiter_whole_suffix); }

template <typename T>
std::string stringify_stl_unordered_set(const std::unordered_set<T> &val,
const std::string &delimiter_whole_prefix="[",const std::string &delimiter_item_prefix="",const std::string &delimiter_item_suffix="",const std::string &delimiter_item_sep=",",const std::string &delimiter_whole_suffix="]"
    ) { return stringify_stl_one<std::unordered_set<T> >(val,
delimiter_whole_prefix,delimiter_item_prefix,delimiter_item_suffix,delimiter_item_sep,delimiter_whole_suffix); }

template <typename T>
std::string stringify_stl_unordered_multiset(const std::unordered_multiset<T> &val,
const std::string &delimiter_whole_prefix="[",const std::string &delimiter_item_prefix="",const std::string &delimiter_item_suffix="",const std::string &delimiter_item_sep=",",const std::string &delimiter_whole_suffix="]"
    ) { return stringify_stl_one<std::unordered_multiset<T> >(val,
delimiter_whole_prefix,delimiter_item_prefix,delimiter_item_suffix,delimiter_item_sep,delimiter_whole_suffix); }

template <typename K, typename T>
std::string stringify_stl_map(const std::map<K, T> &val,
const std::string &delimiter_whole_prefix="{",const std::string &delimiter_key_prefix="",const std::string &delimiter_key_suffix="",const std::string &delimiter_kv_sep=":",const std::string &delimiter_value_prefix="",const std::string &delimiter_value_suffix="",const std::string &delimiter_item_sep=",",const std::string &delimiter_whole_suffix="}"
    ) { return stringify_stl_two<std::map<K, T> >(val,
delimiter_whole_prefix,delimiter_key_prefix,delimiter_key_suffix,delimiter_kv_sep,delimiter_value_prefix,delimiter_value_suffix,delimiter_item_sep,delimiter_whole_suffix); }

template <typename K, typename T>
std::string stringify_stl_multimap(const std::multimap<K, T> &val,
const std::string &delimiter_whole_prefix="{",const std::string &delimiter_key_prefix="",const std::string &delimiter_key_suffix="",const std::string &delimiter_kv_sep=":",const std::string &delimiter_value_prefix="",const std::string &delimiter_value_suffix="",const std::string &delimiter_item_sep=",",const std::string &delimiter_whole_suffix="}"
    ) { return stringify_stl_two<std::multimap<K, T> >(val,
delimiter_whole_prefix,delimiter_key_prefix,delimiter_key_suffix,delimiter_kv_sep,delimiter_value_prefix,delimiter_value_suffix,delimiter_item_sep,delimiter_whole_suffix); }


template <typename K, typename T>
std::string stringify_stl_unordered_map(const std::unordered_map<K, T> &val,
const std::string &delimiter_whole_prefix="{",const std::string &delimiter_key_prefix="",const std::string &delimiter_key_suffix="",const std::string &delimiter_kv_sep=":",const std::string &delimiter_value_prefix="",const std::string &delimiter_value_suffix="",const std::string &delimiter_item_sep=",",const std::string &delimiter_whole_suffix="}"
    ) { return stringify_stl_two<std::unordered_map<K, T> >(val,
delimiter_whole_prefix,delimiter_key_prefix,delimiter_key_suffix,delimiter_kv_sep,delimiter_value_prefix,delimiter_value_suffix,delimiter_item_sep,delimiter_whole_suffix); }

template <typename K, typename T>
std::string stringify_stl_unordered_multimap(const std::unordered_multimap<K, T> &val,
const std::string &delimiter_whole_prefix="{",const std::string &delimiter_key_prefix="",const std::string &delimiter_key_suffix="",const std::string &delimiter_kv_sep=":",const std::string &delimiter_value_prefix="",const std::string &delimiter_value_suffix="",const std::string &delimiter_item_sep=",",const std::string &delimiter_whole_suffix="}"
    ) { return stringify_stl_two<std::unordered_multimap<K, T> >(val,
delimiter_whole_prefix,delimiter_key_prefix,delimiter_key_suffix,delimiter_kv_sep,delimiter_value_prefix,delimiter_value_suffix,delimiter_item_sep,delimiter_whole_suffix); }

};

#endif

