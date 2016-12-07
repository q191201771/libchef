/**
 * @file   assert_wrapper.hpp
 * @deps   nope
 *
 * @author
 *   chef <191201771@qq.com>
 *     -initial release xxxx-xx-xx
 *
 * @brief
 *   重定义debug模式时，assert的行为（由coredump改为往stderr输出日志）。release模式下依然保持原有逻辑（忽略assert）
 *
 */

#ifndef _BASE_TEST_COMMON_ASSERT_WRAPPER_HPP_
#define _BASE_TEST_COMMON_ASSERT_WRAPPER_HPP_

#include <assert.h>
#ifdef NDEBUG
#undef assert
#define assert(expr) if(!(expr)) {fprintf(stderr, "%s %s %d\n", #expr, __FILE__, __LINE__);};
#endif

#endif // _BASE_TEST_COMMON_ASSERT_WRAPPER_HPP_
