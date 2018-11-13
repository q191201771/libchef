/**
 * @tag      v1.5.13
 * @file     chef_lru.hpp
 * @deps     nope
 * @platform linux | macos | xxx
 *
 * @author
 *   chef <191201771@qq.com>
 *     -initial release xxxx-xx-xx
 *
 * @brief    固定大小的LRU cache，支持插入，查询，以及获取全量列表
 *
     ```
     chef::lru<std::string, int> c(3);
     c.put("chef", 1);
     c.put("yoko", 2);
     c.put("tom", 3);
     c.put("jerry", 4); // 超过容器大小，淘汰最老的`chef`
     bool exist;
     int v;
     exist = c.get("chef", &v);
     //assert(!exist);
     exist = c.get("yoko", &v);
     //assert(exist && v == 2);
     c.put("garfield", 5); // 超过容器大小，注意，由于`yoko`刚才读取时会更新热度，所以淘汰的是`tom`
     exist = c.get("yoko", &v);
     //assert(exist && v == 2);
     exist = c.get("tom", &v);
     //assert(!exist);
     ```
 *
 */

#ifndef _CHEF_BASE_LRU_HPP_
#define _CHEF_BASE_LRU_HPP_
#pragma once

#include <map>
#include <list>

namespace chef {

  template <typename KeyT, typename ValueT>
  class lru {
    public:
      typedef std::pair<KeyT, ValueT> KvPair;
      typedef std::list<KvPair> List;

    public:
      // @param cap 容器大小
      explicit lru(std::size_t cap);
      ~lru();

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
      lru(const lru&);
      lru &operator=(const lru&);

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
lru<KeyT, ValueT>::lru(std::size_t cap) : capacity_(cap) {}

template <typename KeyT, typename ValueT>
lru<KeyT, ValueT>::~lru() { list_.clear(); map_.clear(); }

template <typename KeyT, typename ValueT>
bool lru<KeyT, ValueT>::put(KeyT k, ValueT v) {
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
bool lru<KeyT, ValueT>::get(KeyT k, ValueT *v) {
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
std::size_t lru<KeyT, ValueT>::size() const {
  return list_.size();
}

template <typename KeyT, typename ValueT>
std::size_t lru<KeyT, ValueT>::capacity() const {
  return capacity_;
}

template <typename KeyT, typename ValueT>
typename lru<KeyT, ValueT>::List lru<KeyT, ValueT>::get_list() {
  return list_;
}

} // namespace chef

#endif // _CHEF_BASE_LRU_HPP_
