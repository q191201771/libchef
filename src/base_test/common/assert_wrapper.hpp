#ifndef _BASE_TEST_COMMON_ASSERT_WRAPPER_HPP_
#define _BASE_TEST_COMMON_ASSERT_WRAPPER_HPP_

#include <assert.h>
#ifdef NDEBUG
#undef assert
#define assert(expr) if(!(expr)) {fprintf(stderr, "%s %s %d\n", #expr, __FILE__, __LINE__);};
#endif

#endif // _BASE_TEST_COMMON_ASSERT_WRAPPER_HPP_
