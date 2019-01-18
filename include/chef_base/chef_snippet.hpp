/**
 * @license  this file is a part of libchef. more info see https://github.com/q191201771/libchef
 * @tag      v1.10.17
 * @file     chef_snippet.hpp
 * @deps     c++11
 * @platform linux | macos | xxx
 *
 * @author
 *   chef <191201771@qq.com>
 *     -initial release xxxx-xx-xx
 *
 * @brief    用宏减少一些手写重复代码。比如你有一个结构体，里面有各种类型的各种名称的成员变量，有可能有锁或无锁。你不再需要手写这些变量的声明、set、get函数等一堆代码
 *
     ```
     // 简单示例
     struct Foo {
       CHEF_PROPERTY(std::string, name);
       CHEF_PROPERTY(int, age);

       // 附带初始化值
       CHEF_PROPERTY_WITH_INIT_VALUE(int, num_of_child, 128);

       // 我们用std中的mutex来保证score和money的读写是带锁保护的，你也可以选择其他lock_guard类型的锁使用，比如boost，但使用方需在包含该模块之前包含boost相应的头文件
       // 此处score和money共用了一把锁m，使用方也可以根据业务情况自由选择锁粒度
       CHEF_PROPERTY_STD_LOCK(m);
       CHEF_PROPERTY_WITH_STD_LOCK(m, float, score);
       CHEF_PROPERTY_WITH_STD_LOCK(m, long, money);
     };

     Foo f;
     f.set_name("chef");
     f.set_age(18);
     f.set_score(88.8);
     f.set_money(500 * 10000);
     std::cout << f.name() << " " << f.age() << " " << f.score() << " " << f.money() << " " << f.num_of_child() << std::endl;
     ```
 *
 */

#ifndef _CHEF_BASE_SNIPPET_HPP_
#define _CHEF_BASE_SNIPPET_HPP_
#pragma once

#if (__cplusplus >= 201103L)
#include <mutex>
#endif

// @NOTICE 由于以下宏会操作类的访问权限，所以使用方需注意宏之后的类成员函数或变量访问权限可能发生变化。使用方可选择把这些宏放在类的最后或使用完宏后再重新定义访问权限。

// 无锁数据成员
#define CHEF_PROPERTY(Type, name) \
  public: \
    Type name() const { return name##_; } \
    void set_##name(Type v) { name##_ = v; } \
  private: \
    Type name##_;

// 带初始化值的数据成员
#define CHEF_PROPERTY_WITH_INIT_VALUE(Type, name, value) \
  public: \
    Type name() const { return name##_; } \
    void set_##name(Type v) { name##_ = v; } \
  private: \
    Type name##_ = value;

// 锁数据成员
#define CHEF_PROPERTY_LOCK(LockType, lockName) \
  private: \
    LockType lockName##_;

// 带锁的数据成员
#define CHEF_PROPERTY_WITH_LOCK(GuardType, LockType, lockName, PropertyType, propertyName) \
  public: \
    PropertyType propertyName() { GuardType<LockType> l(lockName##_); return propertyName##_; } \
    void set_##propertyName(PropertyType v) { propertyName##_ = v; } \
  private: \
    PropertyType propertyName##_;

// c++11锁
#if (__cplusplus >= 201103L)
#define CHEF_PROPERTY_STD_LOCK(name) CHEF_PROPERTY_LOCK(std::mutex, name)
#define CHEF_PROPERTY_WITH_STD_LOCK(lockName, PropertyType, propertyName) CHEF_PROPERTY_WITH_LOCK(std::lock_guard, std::mutex, lockName, PropertyType, propertyName)
#else
#error("macro CHEF_PROPERTY_STD_LOCK and CHEF_PROPERTY_WITH_STD_LOCK deps on c++11, you should compile it with c++11.")
#endif

// boost锁
#define CHEF_PROPERTY_BOOST_LOCK(name) CHEF_PROPERTY_LOCK(boost::mutex, name)
#define CHEF_PROPERTY_WITH_BOOST_LOCK(lockName, PropertyType, propertyName) CHEF_PROPERTY_WITH_LOCK(boost::lock_guard, boost::mutex, lockName, PropertyType, propertyName)

#endif
