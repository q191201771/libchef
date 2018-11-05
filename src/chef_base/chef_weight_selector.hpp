/**
 * @tag      v1.5.12
 * @file     chef_weight_selector.hpp
 * @deps     nope
 * @platform linux | macos | xxx
 *
 * @author
 *   chef <191201771@qq.com>
 *     -initial release xxxx-xx-xx
 *
 * @brief    往容器中添加带有权重的元素，根据权重轮询（Weight Round-Robin）返回容器中的元素
 *
     ```
     // 简单示例，以下往容器中加入三个节点a b c，对应的权重分别是2 1 4
     // 输出的结果为 c a c b c a c
     chef::weight_selector<std::string> ws;
     ws.add("a", 2);
     ws.add("b", 1);
     ws.add("c", 4);
     std::string val;
     for (int i = 0; i < 7; i++) {
       bool ret = ws.choice_one(val);
       //assert(ret);
       //std::cout << val << " ";
     }
     //std::cout << std::endl;
     ```
 *
 */

#ifndef _CHEF_BASE_WEIGHT_SELECTOR_HPP_
#define _CHEF_BASE_WEIGHT_SELECTOR_HPP_
#pragma once

#include <map>
#include <stdint.h>
#include "chef_stringify_stl.hpp"

namespace chef {

  template <typename T>
  class weight_selector {
    public:
      typedef std::map<T, int64_t> item2weight_t;

    public:
      weight_selector();
      ~weight_selector();

    public:
      /**
       * 添加元素
       *
       * @NOTICE
       *   * 可添加重复的元素，添加后更新之前的权重值
       *   * 权重值设置为0则该元素不会被选取返回，可用该方法摘除故障元素
       *
       */
      void add(T item, uint32_t weight);

      /**
       * @param item 传出参数，当前选取的元素
       * @return true 选取成功
       *         false 选取失败，可能的原因是内部没有元素或所有元素的weight都设置成了0
       *
       */
      bool choice_one(T &item);

    private:
      uint32_t      total_weight_;
      item2weight_t item2weight_initial_;
      item2weight_t item2weight_current_;
  };

} // namespace chef





///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// @NOTICE 该分隔线以上部分为该模块的接口，分割线以下部分为对应的实现





#include <stdlib.h>
#include <iostream>
#include <assert.h>

namespace chef {

template <typename T>
weight_selector<T>::weight_selector() {
  total_weight_ = 0;
}

template <typename T>
weight_selector<T>::~weight_selector() { }

template <typename T>
void weight_selector<T>::add(T item, uint32_t weight) {
  typename item2weight_t::iterator iter = item2weight_initial_.find(item);
  if (iter == item2weight_initial_.end()) {
    total_weight_ += weight;
  } else {
    total_weight_ = total_weight_ - iter->second + weight;
  }

  item2weight_initial_[item] = weight;
  item2weight_current_[item] = weight;
}

template <typename T>
bool weight_selector<T>::choice_one(T &item) {
  if (total_weight_ == 0) { return false; }

  int64_t current_max = -0xFFFFFFF;
  typename item2weight_t::iterator iter_current_max;
  typename item2weight_t::iterator iter_current = item2weight_current_.begin();
  for (; iter_current != item2weight_current_.end(); ++iter_current) {
    if (iter_current->second > current_max && item2weight_initial_[iter_current->first] > 0) {
      current_max = iter_current->second;
      iter_current_max = iter_current;
    }
  }
  assert(iter_current_max != item2weight_current_.end());

  item = iter_current_max->first;

  iter_current_max->second -= total_weight_;

  iter_current = item2weight_current_.begin();
  for (; iter_current != item2weight_current_.end(); ++iter_current) {
    iter_current->second += item2weight_initial_[iter_current->first];
  }

  return true;
}

} // namespace chef

#endif // _CHEF_BASE_WEIGHT_SELECTOR_HPP_
