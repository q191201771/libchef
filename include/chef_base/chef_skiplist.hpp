/**
 * @license  this file is a part of libchef. more info see https://github.com/q191201771/libchef
 * @tag      v1.10.17
 * @file     chef_skiplist.hpp
 * @deps     c++11
 * @platform linux | macos | xxx
 *
 * @author
 *   chef <191201771@qq.com>
 *     -initial release xxxx-xx-xx
 *
 * @brief    跳表
 *
 */

#ifndef _CHEF_BASE_SKIPLIST_HPP_
#define _CHEF_BASE_SKIPLIST_HPP_
#pragma once

#include <stdint.h>
#include <chrono>
#include <functional>
#include <iostream>
#include <random>
#include <vector>

namespace chef {

  template <typename T>
  struct skiplist_default_comparator {
    int operator() (const T &a, const T &b) const {
      if (a > b) { return 1; }
      else if (a == b) { return 0; }
      else { return -1; }
    }
  };

  template <typename _Key, typename _Tp, typename _Compare = skiplist_default_comparator<_Key> >
  class skiplist {
    public:
      using key_type = _Key;
      using mapped_type = _Tp;
      using value_type = std::pair<const _Key, _Tp>;
      using key_compare = _Compare;
    public:
      class iterator;
      class reverse_iterator;

    private:
      struct node;

    public:
      /// empty
      skiplist()
        : head_(new node(max_height_))
        , random_generator_(std::chrono::system_clock::now().time_since_epoch().count()) {}

      /// range
      skiplist(iterator first, iterator last)
        : head_(new node(max_height_))
        , random_generator_(std::chrono::system_clock::now().time_since_epoch().count())
      {
        insert_range_effective_(first, last);
      }

      /// copy
      skiplist(const skiplist &x)
        : head_(new node(max_height_))
        , random_generator_(std::chrono::system_clock::now().time_since_epoch().count())
      {
        insert_range_effective_(x.begin(), x.end());
      }

      ~skiplist() {
        clear();
        delete head_;
      }

      skiplist &operator=(const skiplist &x) {
        clear();
        insert_range_effective_(x.begin(), x.end());
        return *this;
      }

    public: /// Iterators
      iterator begin() { return iterator(head_->levels_[0].next_); }
      iterator begin() const { return iterator(head_->levels_[0].next_); }
      iterator end() { return iterator(nullptr); }
      iterator end() const { return iterator(nullptr); }

      reverse_iterator rbegin() { return reverse_iterator(tail_); }
      reverse_iterator rbegin() const { return reverse_iterator(tail_); }
      reverse_iterator rend() { return reverse_iterator(nullptr); }
      reverse_iterator rend() const { return reverse_iterator(nullptr); }

    public: /// Capacity
      bool empty() const { return element_num_ == 0; }
      std::size_t size() const { return element_num_; }
      // the container is by no means guaranteed to be able to reach that size:
      // it can still fail to allocate storage at any point before that size is reached.
      std::size_t max_size() const { return 0x7FFFFFFF - element_num_; }

    public: /// Element access
      mapped_type &operator[] (const key_type &k) {
        std::vector<node *> cache;
        iterator iter = find_(k, nullptr, nullptr, &cache);
        if (iter != end()) { return iter->second; }

        iter = insert_(std::make_pair(k, mapped_type()), cache);
        return iter->second;
      }

      mapped_type &at(const key_type &k) {
        iterator iter = find_(k);
        if (iter == end()) { throw std::out_of_range("skiplist::at"); }

        return iter->second;
      }

      const mapped_type &at(const key_type &k) const {
        iterator iter = find_(k);
        if (iter == end()) { throw std::out_of_range("skiplist::at"); }

        return iter->second;
      }

    public: /// Modifiers
      /// single element
      std::pair<iterator, bool> insert(const value_type &val) {
        std::vector<node *> cache;
        iterator iter = find_(val.first, nullptr, nullptr, &cache);
        if (iter != end()) { return std::make_pair(iter, false); }

        iter = insert_(val, cache);
        return std::make_pair(iter, true);
      }

      /// with hint
      iterator insert(iterator position, const value_type &val) {
        std::vector<node *> cache;
        iterator iter = find_(val.first, position.node_, nullptr, &cache);
        if (iter != end()) { return iter; }

        iter = find_(val.first, nullptr, position.node_, &cache);
        if (iter != end()) { return iter; }

        return insert_(val, cache);
      }

      /// range
      void insert(iterator first, iterator last) { insert_range_effective_(first, last); }

      void erase(iterator position) { erase(position->first); }

      std::size_t erase(const key_type &k) {
        iterator iter = find_(k);
        if (iter == end()) { return 0; }

        return erase_(iter.node_);
      }

      void erase(iterator first, iterator last) {
        for (iterator iter = first; iter != last;) { erase_((iter++).node_); }
      }

      void swap(skiplist &x) {
        std::swap(head_, x.head_);
        std::swap(current_height_, x.current_height_);
        std::swap(element_num_, x.element_num_);
      }

      void clear() {
        for (int i = max_height_-1; i > 0; i--) { head_->levels_[i].next_ = nullptr; }

        node *p = head_->levels_[0].next_;
        while (p) {
          node *next = p->levels_[0].next_;
          delete p;
          p = next;
        }
        head_->levels_[0].next_ = nullptr;
        current_height_ = 1;
        element_num_ = 0;
      }

    public: /// Operations
      iterator find(const key_type &k) const { return find_(k); }

      std::size_t count(const key_type &k) const { return find_(k) == end() ? 0 : 1; }

      iterator lower_bound(const key_type &k) const {
        std::vector<node *> cache;
        iterator iter = find_(k, nullptr, nullptr, &cache);
        if (iter != end()) { return iter; }

        return iterator(cache[0]->levels_[0].next_);
      }

      iterator upper_bound(const key_type &k) const {
        std::vector<node *> cache;
        iterator iter = find_(k, nullptr, nullptr, &cache);
        if (iter != end()) { return ++iter; }

        return iterator(cache[0]->levels_[0].next_);
      }

      std::pair<iterator, iterator> equal_range(const key_type &k) const {
        std::vector<node *> cache;
        iterator iter = find_(k, nullptr, nullptr, &cache);
        if (iter != end()) { return std::make_pair(iter, iterator(iter.node_->levels_[0].next_)); }

        iterator res(cache[0]->levels_[0].next_);
        return std::make_pair(res, res);
      }

    private:
      iterator find_(const key_type &k, node *bpos=nullptr, node *epos=nullptr, std::vector<node *> *cache=nullptr) const {
        if (cache) { cache->reserve(max_height_); }

        node *dummy = bpos ? bpos->levels_[0].prev_ : head_;
        int h = bpos ? dummy->height_ : current_height_;

        for (int i = h-1; i >= 0; i--) {
          for (; ; ) {
            if (epos == nullptr && dummy->levels_[i].next_ == nullptr) { break; }
            else if (epos != nullptr && (dummy->levels_[i].next_ == epos || dummy->levels_[i].next_ == nullptr)) { break; }

            int res = key_compare()(k, dummy->levels_[i].next_->kv_.first);
            if (res == 0) { return iterator(dummy->levels_[i].next_); }
            else if (res < 0) { break; }
            else if (res > 0) { dummy = dummy->levels_[i].next_; }
          }
          if (cache) { (*cache)[i] = dummy; }
        }

        return end();
      }

      iterator insert_(const value_type &kv, std::vector<node *> &cache) {
        int height = random_height_();
        if (height > current_height_) {
          for (int i = height-1; i >= current_height_; i--) {
            cache[i] = head_;
          }
          current_height_ = height;
        }

        node *nn = new node(height, kv);
        for (int i = height-1; i >= 0; i--) {
          node *cn = cache[i];
          nn->levels_[i].next_ = cn->levels_[i].next_;
          nn->levels_[i].prev_ = cn;
          if (cn->levels_[i].next_) { cn->levels_[i].next_->levels_[i].prev_ = nn; }

          cn->levels_[i].next_ = nn;
        }

        if (nn->levels_[0].next_ == nullptr) { tail_ = nn; }

        element_num_++;
        return iterator(nn);
      }

      std::size_t erase_(node *n) {
        for (int i = n->height_-1; i >= 0; i--) {
          n->levels_[i].prev_->levels_[i].next_ = n->levels_[i].next_;
          if (n->levels_[i].next_) { n->levels_[i].next_->levels_[i].prev_ = n->levels_[i].prev_; }
        }

        if (n->height_ != 1 && n->height_ == current_height_) {
          for (; (current_height_ > 1) && (head_->levels_[current_height_-1].next_ == nullptr); current_height_--);
        }

        if (n == tail_) { tail_ = n->levels_[0].prev_ == head_ ? nullptr : n->levels_[0].prev_; }

        element_num_--;
        delete n;
        return 1;
      }

    private:
      void insert_range_effective_(iterator b, iterator e) {
        iterator pos;
        std::vector<node *> cache;
        for (iterator iter = b; iter != e; ++iter) {
          pos = find_(iter->first, pos.node_, nullptr, &cache);
          if (pos == end()) { pos = insert_(*iter, cache); }
        }
      }

    public:

#define CHEF_SKIPLIST_ITERATOR_COMMON_SNIPPET(T) \
  public: \
    friend class skiplist; \
  public: \
    T() = default; \
    explicit T(node *n) : node_(n) {} \
    T(const T &other) { node_ = other.node_; } \
    T &operator=(const T &other) { \
      node_ = other.node_; \
      return *this; \
    } \
    bool operator==(const T &other) const { return this->node_ == other.node_; } \
    bool operator!=(const T &other) const { return this->node_ != other.node_; } \
    value_type &operator*() const { return node_->kv_; } \
    value_type *operator->() const { return &(node_->kv_); } \
  private: \
    node *node_ = nullptr;
/// } end of macro CHEF_SKIPLIST_ITERATOR_COMMON_SNIPPET

    public:
      class iterator {
        CHEF_SKIPLIST_ITERATOR_COMMON_SNIPPET(iterator);

        public:
          iterator &operator++() {
            node_ = node_->levels_[0].next_;
            return *this;
          }

          iterator operator++(int) {
            node *keeper = node_;
            node_ = node_->levels_[0].next_;
            return iterator(keeper);
          }

          iterator &operator--() {
            node_ = node_->levels_[0].prev_;
            return *this;
          }

          iterator operator--(int) {
            node *keeper = node_;
            node_ = node_->levels_[0].prev_;
            return iterator(keeper);
          }
      }; // class iterator

      class reverse_iterator {
        CHEF_SKIPLIST_ITERATOR_COMMON_SNIPPET(reverse_iterator);

        public:
          reverse_iterator &operator++() {
            node_ = node_->levels_[0].prev_;
            if (node_->levels_[0].prev_ == nullptr) { node_ = nullptr; }

            return *this;
          }

          reverse_iterator operator++(int) {
            node *keeper = node_;
            node_ = node_->levels_[0].prev_;
            if (node_->levels_[0].prev_ == nullptr) { node_ = nullptr; }

            return reverse_iterator(keeper);
          }

          reverse_iterator &operator--() {
            node_ = node_->levels_[0].next_;
            return *this;
          }

          reverse_iterator operator--(int) {
            node *keeper = node_;
            node_ = node_->levels_[0].next_;
            return reverse_iterator(keeper);
          }
      }; //class reverse_iterator

      // TODO const_iterator

    private:
      int random_height_() {
        int height = 1;
        for (; height < max_height_ && (random_generator_() % branching_ == 0); height++);

        return height;
      }

    private:
      struct level_item {
        node *prev_ = nullptr;
        node *next_ = nullptr;
      };

      struct node {
        public:
          friend class skiplist;

        public:
          explicit node(int height)
            : height_(height), levels_(height) {}

          explicit node(int height, const value_type &kv)
            : kv_(kv), height_(height), levels_(height) {}

        private:
          value_type kv_;
          int height_;
          std::vector<level_item> levels_;
      };

    private:
      static constexpr int max_height_ = 12;
      static constexpr int branching_ = 4;
      int current_height_ = 1;
      std::size_t element_num_ = 0;
      node *tail_ = nullptr;
      node *head_;
      std::mt19937 random_generator_;
      //node *cache_[max_height_];

    public:
      void debug_print() {
        std::cout << "<<<<<" << std::endl;
        for (int i = current_height_-1; i >= 0; i--) {
          std::cout << " * ";
          node *p = head_;
          for (; p->levels_[i].next_; p = p->levels_[i].next_) {
            std::cout << p->levels_[i].next_->kv_.first << " ";
          }
          std::cout << std::endl;
        }
        std::cout << "-----" << std::endl;
        chef::skiplist<int, int>::iterator iter;
        iter = begin();
        for (; iter != end(); ++iter) {
          std::cout << "(" << iter->first << "," << iter->second << ") ";
        }
        std::cout << std::endl
                  << ">>>>>" << std::endl;
      }

  }; // skip_list

} // namespace chef

#endif
