/**
 * @git      https://github.com/q191201771/starry-night
 * @tag      v1.7.17
 * @file     chef_consistent_hash.hpp
 * @deps     c++11
 * @platform linux | macos | xxx
 *
 * @author
 *   chef <191201771@qq.com>
 *     -initial release xxxx-xx-xx
 *
 * @brief    一致性哈希
 *
 */

#ifndef _CHEF_BASE_CONSISTENT_HASH_HPP_
#define _CHEF_BASE_CONSISTENT_HASH_HPP_
#pragma once

#include <functional>
#include <map>
#include <iostream>
#include <set>
#include <sstream>
#include <string>
#include <stdint.h>

namespace chef {

  // VirtualMultis为生成虚拟节点的倍数
  template <typename T, int VirtualMultis>
  class consistent_hash {
    public:
      void add_node(const T &node);
      void del_node(const T &node);
      // 如果容器为空，返回false
      bool choice_one_node(const T &key, T &node /* out */);

    public:
      bool is_node_exist(const T &node);
      std::set<T> get_all_nodes();
      void clear_all_nodes();

    private:
      typedef std::size_t hash_result_t;

    private:
      std::set<T>                nodes_;
      std::map<hash_result_t, T> pos2node_;

    public:
      consistent_hash() {}
    private:
      consistent_hash(const consistent_hash &);
      consistent_hash &operator=(const consistent_hash &);

  }; // class consistent_hash

} // namespace chef





///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// @NOTICE 该分隔线以上部分为该模块的接口，分割线以下部分为对应的实现





namespace chef {

  template <typename T, int VirtualMultis>
  void consistent_hash<T, VirtualMultis>::add_node(const T &node) {
    if (nodes_.find(node) != nodes_.end()) { return; }

    nodes_.insert(node);
    for (uint32_t i = 0; i < VirtualMultis; i++) {
      std::ostringstream hks;
      hks << node << "__" << i;
      std::size_t hv = std::hash<std::string>()(hks.str());
      pos2node_[hv] = node;
    }
  }

  template <typename T, int VirtualMultis>
  void consistent_hash<T, VirtualMultis>::del_node(const T &node) {
    if (nodes_.empty()) { return; }

    nodes_.erase(node);
    for (uint32_t i = 0; i < VirtualMultis; i++) {
      std::ostringstream hks;
      hks << node << "__" << i;
      std::size_t hv = std::hash<std::string>()(hks.str());
      pos2node_.erase(hv);
    }
  }

  template <typename T, int VirtualMultis>
  bool consistent_hash<T, VirtualMultis>::choice_one_node(const T &key, T &node) {
    if (nodes_.empty()) { return false; }

    hash_result_t hv = std::hash<T>()(key);
    typename std::map<hash_result_t, T>::iterator iter = pos2node_.begin();
    for (; iter != pos2node_.end(); ++iter) {
      if (hv < iter->first) {
        node = iter->second;
        return true;
      }
    }
    node = pos2node_.begin()->second;
    return true;
  }

  template <typename T, int VirtualMultis>
  bool consistent_hash<T, VirtualMultis>::is_node_exist(const T &node) {
    return nodes_.find(node) != nodes_.end();
  }

  template <typename T, int VirtualMultis>
  std::set<T> consistent_hash<T, VirtualMultis>::get_all_nodes() {
    return nodes_;
  }

  template <typename T, int VirtualMultis>
  void consistent_hash<T, VirtualMultis>::clear_all_nodes() {
    nodes_.clear();
    pos2node_.clear();
  }
} // namespace chef

#endif
