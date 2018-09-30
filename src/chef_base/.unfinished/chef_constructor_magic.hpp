/**
 * @file     chef_constructor_magic.hpp
 * @deps     nope
 * @platform linux/macos/xxx
 *
 * @author
 *   chef <191201771@qq.com>
 *     -initial release xxxx-xx-xx
 *
 * @brief  宏定义方便的禁用掉类赋值函数、拷贝构造函数、默认构造函数
 *
 */

#ifndef _CHEF_BASE_CONSTRUCTOR_MAGIC_HPP_
#define _CHEF_BASE_CONSTRUCTOR_MAGIC_HPP_


#if (__cplusplus >= 201103)
  #define CHEF_DISALLOW_ASSIGN(TypeName) \
    void operator=(const TypeName &) = delete;

  #define CHEF_DISALLOW_COPY_AND_ASSIGN(TypeName) \
    CHEF_DISALLOW_ASSIGN(TypeName); \
    TypeName(const TypeName &) = delete;

  #define CHEF_DISALLOW_IMPLICIT_CONSTRUCTORS(TypeName) \
    CHEF_DISALLOW_COPY_AND_ASSIGN(TypeName); \
    TypeName() = delete;

#else
  // @NOTICE 建议放在类内声明的底部，避免private污染下面的空间

  #define CHEF_DISALLOW_ASSIGN(TypeName) \
    private: void operator=(const TypeName &);

  #define CHEF_DISALLOW_COPY_AND_ASSIGN(TypeName) \
    CHEF_DISALLOW_ASSIGN(TypeName); \
    TypeName(const TypeName &);

  #define CHEF_DISALLOW_IMPLICIT_CONSTRUCTORS(TypeName) \
    CHEF_DISALLOW_COPY_AND_ASSIGN(TypeName); \
    TypeName();

#endif // __cplusplus

#endif // _CHEF_BASE_CONSTRUCTOR_MAGIC_HPP_
