/**
 * @tag      v1.3.3
 * @file     chef_lru.hpp
 * @deps     nope
 * @platform linux/macos/xxx
 *
 * @author
 *   chef <191201771@qq.com>
 *     -initial release xxxx-xx-xx
 *
 * @brief    固定大小的LRU cache，支持插入，查询，以及获取全量列表
 *
 */

#ifndef _CHEF_BASE_LRU_HPP_
#define _CHEF_BASE_LRU_HPP_
#pragma once

#include <map>
#include <list>

namespace chef {

  template <typename KeyT, typename ValueT>
  class Lru {
    public:
      typedef std::pair<KeyT, ValueT> KvPair;
      typedef std::list<KvPair> List;

    public:
      // @param cap 容器大小
      Lru(std::size_t cap);
      ~Lru();

    public:
      // @NOTICE function put 和 function get 操作都会更新元素热度，put 的 key 即使已经存在甚至对应的 value 相同也会更新热度

      // 插入前k不存在返回true，否则返回false
      bool put(KeyT k, ValueT v);

      // k存在返回true，否则false
      bool get(KeyT k, ValueT *v);

      // 获取整个列表
      List get_list();

      std::size_t size() const;
      std::size_t capacity() const;

    private:
      Lru(const Lru&);
      Lru &operator=(const Lru&);

    private:
      typedef std::map<KeyT, typename List::iterator> Map;

    private:
      const std::size_t capacity_;
      List              list_;
      Map               map_;

  }; // class lru

} // namespace chef





///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// @NOTICE 该分隔线以上部分为该模块的接口，分割线以下部分为对应的实现




namespace chef {

template <typename KeyT, typename ValueT>
Lru<KeyT, ValueT>::Lru(std::size_t cap) : capacity_(cap) {}

template <typename KeyT, typename ValueT>
Lru<KeyT, ValueT>::~Lru() { list_.clear(); map_.clear(); }

template <typename KeyT, typename ValueT>
bool Lru<KeyT, ValueT>::put(KeyT k, ValueT v) {
  bool not_exist = true;
  typename Map::iterator iter = map_.find(k);
  if (iter != map_.end()) {
    list_.erase(iter->second);
    map_.erase(iter);
    not_exist = false;
  }

  list_.push_front(std::make_pair(k, v));
  map_[k] = list_.begin();

  if (list_.size() > capacity_) {
    KeyT old = list_.back().first;
    list_.pop_back();
    map_.erase(old);
  }
  return not_exist;
}

template <typename KeyT, typename ValueT>
bool Lru<KeyT, ValueT>::get(KeyT k, ValueT *v) {
  typename Map::iterator iter = map_.find(k);
  if (iter == map_.end()) {
    return false;
  }
  KvPair kvp = *(iter->second);
  list_.erase(iter->second);
  list_.push_front(kvp);
  map_[k] = list_.begin();
  *v = kvp.second;
  return true;
}

template <typename KeyT, typename ValueT>
std::size_t Lru<KeyT, ValueT>::size() const {
  return list_.size();
}

template <typename KeyT, typename ValueT>
std::size_t Lru<KeyT, ValueT>::capacity() const {
  return capacity_;
}

template <typename KeyT, typename ValueT>
typename Lru<KeyT, ValueT>::List Lru<KeyT, ValueT>::get_list() {
  return list_;
}

} // namespace chef

#endif
