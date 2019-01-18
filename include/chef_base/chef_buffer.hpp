/**
 * @license  this file is a part of libchef. more info see https://github.com/q191201771/libchef
 * @tag      v1.10.17
 * @file     chef_buffer.hpp
 * @deps     nope
 * @platform linux | macos | xxx
 *
 * @author
 *   chef <191201771@qq.com>
 *     - initial release 2016-08-31
 *
 * @brief    FIFO的流式buffer，支持自动扩容、收缩，供生产和消费长度不固定的场景使用（例如tcp的读写buffer）
 *
     ```
     // 声明对象
     chef::basic_buffer<char> buf(4096, 1024 * 16);

     // 写入方式一，如果内部空余空间不足，会自动扩容
     buf.append("hello", 5);

     // 写入方式二，先调用reserve确保内部空余空间足够，再直接操作写位置指针，最后手动后移新写位置。
     // 提供方法二是为了便于与其他字符串函数配合，在某些场景下减少一次内存拷贝。例如：
     buf.reserve(128);
     int len = snprintf(buf.write_pos(), 128, "name=%s,age=%d.", "chef", 18);
     buf.seek_write_pos(std::min(len, 128));

     // 读取所有数据
     len = buf.readable_size();
     std::string str(buf.read_pos(), len);
     buf.erase(len);
     // 读取部分数据，业务方可能有这种消费场景，比如
     // loop:
     //   if send buf not empty and event active:
     //     sent_len = tcp_send(fd, buf.read_pos(), buf.readable_size());
     //     buf.erase(sent_len);
     ```
 *
 */

#ifndef _CHEF_BASE_BUFFER_HPP_
#define _CHEF_BASE_BUFFER_HPP_
#pragma once

#include <cinttypes>
#include <cstddef>

namespace ut { class buffer_test; }

namespace chef {

  template <typename DataType, typename LenType>
  class basic_buffer;
  typedef basic_buffer<uint8_t, std::size_t> buffer;

  // NOTICE basic buffer类中所有LenType类型变量都应该为非负数，这里提供出模板类型是为了方便业务方在某些时候不用写类型转换，，
  template <typename DataType=uint8_t, typename LenType=std::size_t>
  class basic_buffer {
    public:
      /**
       * @param init_capacity   初始化大小，后续不够时内部会自动扩容，两倍增长
       * @param shrink_capacity 收缩阀值，当申请内存大于<shrink_capacity>且实际使用小于<init_capacity>时会释发多余内存，
       *                        恢复成<init_capacity>
       *
       */
      explicit basic_buffer(LenType init_capacity=16384, LenType shrink_capacity=1048576);

      /**
       * chef::basic_buffer(data, len);
       * 等价于
       * chef::basic_buffer buf(len, 2 * len);
       * buf.append(data, len);
       *
       * @param data 初始化数据，内部做内存拷贝
       * @param len  初始化数据长度
       *
       */
      basic_buffer(const DataType *data, LenType len);

      /// 析构，释放内部内存
      ~basic_buffer();

      /// 拷贝构造以及赋值函数，内部执行深拷贝
      basic_buffer(const basic_buffer &b);
      basic_buffer &operator=(const basic_buffer &b);

    public:
      /**
       * 追加数据，空余空间不足时自动扩容
       *
       * @param data 追加数据，内部做内存拷贝
       * @param len  追加数据长度
       *
       */
      void append(const DataType *data, LenType len);

      /**
       * @function reserve
       * @param    len     若内部空余空间小于<len>则会扩容新空间
       *
       * @function write_pos 返回写位置
       *
       * @function seek_write_pos 往后挪动写位置
       *
       */
      void reserve(LenType len);
      DataType *write_pos() const;
      void seek_write_pos(LenType len);

      // 将写位置往前移动，丢弃尾部部分数据
      void seek_write_pos_rollback(LenType len);

    public:
      /**
       * 读取buffer中的数据
       *
       */
      DataType *read_pos() const;
      LenType readable_size() const;
      /// 注意，<len>应不大于readable_size函数的返回值
      void erase(LenType len);

      /**
       * 清空，注意：
       * 1. 并不会释放内部内存，只是将空间全部标记为空闲，内部申请的内存只有在析构时释放
       * 2. 如果capacity已经大于shrink阈值了，则收缩成init capacity大小
       *
       */
      void clear();

      /// 已申请内存大小
      LenType capacity() const { return capacity_; }

    public:
      /**
       * @return 找到返回key位置，失败返回NULL
       *
       */
      DataType *find(const DataType *key, LenType len) const;
      DataType *find(DataType c) const;
      DataType *find_crlf() const;
      DataType *find_eol() const;

      /**
       * 删除 '空格' '\f' '\r' '\n' '\t' '\v'
       *
       * @return 返回操作后的读取位置
       *
       */
      DataType *trim_left();
      DataType *trim_right();

    public:
      friend class ut::buffer_test;

    private:
      const LenType  init_capacity_;
      const LenType  shrink_capacity_;
      LenType        capacity_;
      LenType        read_index_;
      LenType        write_index_;
      DataType      *data_;

  }; // class basic_buffer

} // namespace chef





///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// @NOTICE 该分隔线以上部分为该模块的接口，分割线以下部分为对应的实现





#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include <stdarg.h>
#include <assert.h>
#include <algorithm>
#include <cctype>

namespace chef {

  template <typename DataType, typename LenType>
  inline basic_buffer<DataType, LenType>::basic_buffer(LenType init_capacity, LenType shrink_capacity)
    : init_capacity_(init_capacity)
    , shrink_capacity_(shrink_capacity)
    , capacity_(init_capacity)
    , read_index_(0)
    , write_index_(0)
  {
    data_ = new DataType[init_capacity];
  }

  template <typename DataType, typename LenType>
  inline basic_buffer<DataType, LenType>::basic_buffer(const DataType *data, LenType len)
    : init_capacity_(len)
    , shrink_capacity_(len * 2)
    , capacity_(len)
    , read_index_(0)
    , write_index_(0)
  {
    data_ = new DataType[capacity_];
    append(data, len);
  }

  template <typename DataType, typename LenType>
  inline basic_buffer<DataType, LenType>::~basic_buffer() {
    delete []data_;
    data_ = NULL;
  }

  template <typename DataType, typename LenType>
  inline basic_buffer<DataType, LenType>::basic_buffer(const basic_buffer<DataType, LenType> &b)
    : init_capacity_(b.init_capacity_)
    , shrink_capacity_(b.shrink_capacity_)
    , capacity_(b.capacity_)
    , read_index_(0)
    , write_index_(0)
  {
    data_ = new DataType[capacity_];
    append(b.read_pos(), b.readable_size());
  }

  template <typename DataType, typename LenType>
  inline basic_buffer<DataType, LenType> &basic_buffer<DataType, LenType>::operator=(const chef::basic_buffer<DataType, LenType> &b) {
    if (this != &b) {
      read_index_ = 0;
      write_index_ = 0;
      this->append(b.read_pos(), b.readable_size());
    }
    return *this;
  }

  template <typename DataType, typename LenType>
  inline void basic_buffer<DataType, LenType>::append(const DataType *data, LenType len) {
    reserve(len);
    memcpy(data_ + write_index_, data, len);
    write_index_ += len;
  }

  template <typename DataType, typename LenType>
  inline void basic_buffer<DataType, LenType>::reserve(LenType len) {
    if (capacity_ - write_index_ >= len) {
      return;
    } else if (capacity_ - write_index_ + read_index_ >= len) {
      memmove(data_, data_ + read_index_, write_index_ - read_index_);
    } else {
      LenType need_len = write_index_ - read_index_ + len;
      for (; capacity_ < need_len; capacity_ <<= 1);
      DataType *new_buf = new DataType[capacity_];
      memcpy(new_buf, data_ + read_index_, write_index_ - read_index_);
      delete []data_;
      data_ = new_buf;
    }
    write_index_ -= read_index_;
    read_index_ = 0;
  }

  template <typename DataType, typename LenType>
  inline DataType *basic_buffer<DataType, LenType>::write_pos() const { return data_ + write_index_; }

  template <typename DataType, typename LenType>
  inline void basic_buffer<DataType, LenType>::erase(LenType len) {
    assert(write_index_ - read_index_ >= len);
    read_index_ += len;
    if (write_index_ - read_index_ < init_capacity_ &&
        capacity_ > shrink_capacity_)
    {
      DataType *new_data = new DataType[init_capacity_];
      memcpy(new_data, data_ + read_index_, write_index_ - read_index_);
      write_index_ -= read_index_;
      read_index_ = 0;
      delete []data_;
      data_ = new_data;
      capacity_ = init_capacity_;
    }
  }

  template <typename DataType, typename LenType>
  inline void basic_buffer<DataType, LenType>::clear() {
    read_index_ = write_index_ = 0;
    if (capacity_ > shrink_capacity_) {
      capacity_ = init_capacity_;
      delete []data_;
      data_ = new DataType[capacity_];
    }
  }

  template <typename DataType, typename LenType>
  inline void basic_buffer<DataType, LenType>::seek_write_pos(LenType len) {
    assert(capacity_ - write_index_ >= len);
    write_index_ += len;
  }

  template <typename DataType, typename LenType>
  inline void basic_buffer<DataType, LenType>::seek_write_pos_rollback(LenType len) {
    assert(write_index_ - read_index_ >= len);
    write_index_ -= len;
    // may be shrink here.
  }

  template <typename DataType, typename LenType>
  inline DataType *basic_buffer<DataType, LenType>::read_pos() const { return data_ + read_index_; }

  template <typename DataType, typename LenType>
  inline LenType basic_buffer<DataType, LenType>::readable_size() const { return write_index_ - read_index_; }

  template <typename DataType, typename LenType>
  inline DataType *basic_buffer<DataType, LenType>::find(const DataType *key, LenType len) const {
    if (readable_size() == 0) {
      return NULL;
    }
    DataType *pos = std::search(read_pos(), write_pos(), const_cast<DataType *>(key),
      const_cast<DataType *>(key) + len
    );
    return pos == data_ + write_index_ ? NULL : pos;
  }

  template <typename DataType, typename LenType>
  inline DataType *basic_buffer<DataType, LenType>::find(DataType c) const {
    if (readable_size() == 0) {
      return NULL;
    }
    return static_cast<DataType *>(memchr(read_pos(), c, readable_size()));
  }

  template <typename DataType, typename LenType>
  inline DataType *basic_buffer<DataType, LenType>::find_crlf() const { return find((const DataType *)"\r\n", 2); }

  template <typename DataType, typename LenType>
  inline DataType *basic_buffer<DataType, LenType>::find_eol() const { return find('\n'); }

  template <typename DataType, typename LenType>
  inline DataType *basic_buffer<DataType, LenType>::trim_left() {
    for (; write_index_ != read_index_; ++read_index_) {
      DataType ch = *(data_ + read_index_);
      if (!std::isspace(ch)) {
        break;
      }
    }
    return read_pos();
  }

  template <typename DataType, typename LenType>
  inline DataType *basic_buffer<DataType, LenType>::trim_right() {
    for (; write_index_ != read_index_; --write_index_) {
      DataType ch = *(data_ + write_index_ - 1);
      if (!std::isspace(ch)) {
        break;
      }
    }
    return read_pos();
  }

} // namespace chef

#endif // _CHEF_BASE_BUFFER_HPP_
