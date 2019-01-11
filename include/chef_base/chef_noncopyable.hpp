/**
 * @license  this file is a part of libchef. more info see https://github.com/q191201771/libchef
 * @tag      v1.9.17
 * @file     chef_noncopyable.hpp
 * @deps     nope
 * @platform linux/macos/xxx
 *
 * @author
 *   chef <191201771@qq.com>
 *     - initial release xxxx-xx-xx
 *
 * @brief
 *   - class noncopyable  继承这个类，禁用拷贝构造函数、赋值函数
 *   - class copyable     空接口，继承这个类起注释强调作用说明类是可拷贝的
 *   - class static_class 静态可继承这个类，禁用构造函数、拷贝构造函数、赋值函数
 *
 */

#ifndef _CHEF_BASE_NONCOPYABLE_HPP_
#define _CHEF_BASE_NONCOPYABLE_HPP_
#pragma once

namespace chef {

  class noncopyable {
    protected:
      noncopyable() {}
      ~noncopyable() {}

    private:
      noncopyable(const noncopyable &);
      noncopyable &operator=(const noncopyable &);
  };

  class copyable {};

  class static_class {
    protected:
      ~static_class() {}

    private:
      static_class();
      static_class(const static_class &);
      const static_class &operator=(const static_class &);

  };

} // namespace chef

#endif // _CHEF_BASE_NONCOPYABLE_HPP_
