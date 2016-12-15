/**
 * @file   assert_wrapper.hpp
 * @deps   nope
 *
 * @author
 *   chef <191201771@qq.com>
 *     -initial release xxxx-xx-xx
 *
 * @brief
 *   重定义assert的行为（由coredump改为往stderr输出日志）。
 *   使用场景：做ut时，希望assert失败后只爆出一条日志，程序继续执行后续的case。
 *   请勿在生产环境直接使用该wrapper！
 *
 */

#ifndef _BASE_TEST_COMMON_ASSERT_WRAPPER_HPP_
#define _BASE_TEST_COMMON_ASSERT_WRAPPER_HPP_

#include <assert.h>
//#ifndef NDEBUG
#undef assert
#define assert(expr) if(!(expr)) {fprintf(stderr, "%s %s %d\n", #expr, __FILE__, __LINE__);};
//#endif

#endif // _BASE_TEST_COMMON_ASSERT_WRAPPER_HPP_
