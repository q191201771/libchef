/**
 * @license  this file is part of starry-night library. more info see https://github.com/q191201771/starry-night
 * @tag      v1.7.17
 * @file     chef_hash.hpp
 * @deps     c++11
 * @platform linux | macos | xxx
 *
 * @author
 *   chef <191201771@qq.com>
 *     -initial release xxxx-xx-xx
 *
 * @brief
 *   利用变参模板和std::hash实现的万能哈希
 *   可通过多种类型的多个变量组合生成哈希值，使得哈希结果更均匀
 *
     ```
     struct student {
       public:
         student(int id, const std::string &name, int age)
           : id_(id)
           , name_(name)
           , age_(age)
         {}

         size_t hash_val() const { return chef::hash_val(id_, name_, age_); }

       private:
         int         id_;
         std::string name_;
         int         age_;
     };
     ```
 *
 */

#ifndef _CHEF_BASE_HASH_HPP_
#define _CHEF_BASE_HASH_HPP_
#pragma once

#include <functional>

namespace chef {

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

  template <typename... Types>
  inline size_t hash_val(const Types&... args) {
    size_t seed = 0;
    inner::hash_val(seed, args...);
    return seed;
  }

} // namespace chef

#endif // _CHEF_BASE_HASH_HPP_
