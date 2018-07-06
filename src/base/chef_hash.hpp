/**
 * @file   hash.hpp
 * @deps   c++11
 * @platform linux/macos/xxx
 *
 * @author
 *   chef <191201771@qq.com>
 *     -initial release xxxx-xx-xx
 *
 * @brief
 *   利用变参模板实现万能哈希函数
 *   按候捷老师课上讲的写的~
 *
 */

#ifndef _CHEF_BASE_HASH_HPP_
#define _CHEF_BASE_HASH_HPP_
#pragma once

#include <functional>

namespace inner {

  /// everyone
  template <typename T>
  inline void hash_combine(size_t& seed, const T& val) {
    seed ^= std::hash<T>()(val) + 0x9e3779b9 + (seed<<6) + (seed>>2);
  }

  /// last
  template <typename T>
  inline void hash_val(size_t& seed, const T& val) {
    hash_combine(seed, val);
  }

  /// recursive
  template <typename T, typename... Types>
  inline void hash_val(size_t& seed, const T& val, const Types&... args) {
    hash_combine(seed, val);
    hash_val(seed, args...);
  }

} // namespace inner

namespace chef {

  template <typename... Types>
  inline size_t hash_val(const Types&... args) {
    size_t seed = 0;
    inner::hash_val(seed, args...);
    return seed;
  }

} // namespace chef

#endif // _CHEF_BASE_HASH_HPP_
