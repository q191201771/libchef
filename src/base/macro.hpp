/**
 * @file   macro.hpp
 * @deps   nope
 *
 * @author
 *   pengrenliang <renliang.prl@alibaba.com>
 *     -initial release xxxx-xx-xx
 *
 * @brief  一些宏定义，用于减少老工程中的冗余代码
 *
 */

#ifndef _BASE_MACRO_H_
#define _BASE_MACRO_H_

#define _INNER_IF_TRUE_RETURN_(cond, retval) { if (cond) { return retval; } }

#define IF_FALSE_RETURN_VOID(x)         _INNER_IF_TRUE_RETURN_(!(x), ;)
#define IF_FALSE_RETURN_NULL(x)         _INNER_IF_TRUE_RETURN_(!(x), NULL)
#define IF_FALSE_RETURN_FALSE(x)        _INNER_IF_TRUE_RETURN_(!(x), false)
#define IF_FALSE_RETURN_NEGATIVE_ONE(x) _INNER_IF_TRUE_RETURN_(!(x), -1)
#define IF_FALSE_RETURN_VAL(x, retval)  _INNER_IF_TRUE_RETURN_(!(x), retval)

#define IF_NULL_RETURN_VOID(x)         _INNER_IF_TRUE_RETURN_((x) == NULL, ;)
#define IF_NULL_RETURN_NULL(x)         _INNER_IF_TRUE_RETURN_((x) == NULL, NULL)
#define IF_NULL_RETURN_FALSE(x)        _INNER_IF_TRUE_RETURN_((x) == NULL, false)
#define IF_NULL_RETURN_NEGATIVE_ONE(x) _INNER_IF_TRUE_RETURN_((x) == NULL, -1)
#define IF_NULL_RETURN_VAL(x, retval)  _INNER_IF_TRUE_RETURN_((x) == NULL, retval)

#define IF_NEGATIVE_ONE_RETURN_VOID(x)         _INNER_IF_TRUE_RETURN_((x) == -1, ;)
#define IF_NEGATIVE_ONE_RETURN_NULL(x)         _INNER_IF_TRUE_RETURN_((x) == -1, NULL)
#define IF_NEGATIVE_ONE_RETURN_FALSE(x)        _INNER_IF_TRUE_RETURN_((x) == -1, false)
#define IF_NEGATIVE_ONE_RETURN_NEGATIVE_ONE(x) _INNER_IF_TRUE_RETURN_((x) == -1, -1)
#define IF_NEGATIVE_ONE_RETURN_VAL(x, retval)  _INNER_IF_TRUE_RETURN_((x) == -1, retval)

#define FREE_PTR(p)         { if ((p) != NULL) { free(p); (p) = NULL; } }
#define DELETE_PTR(p)       { if ((p) != NULL) { delete (p); (p) = NULL; } }
#define DELETE_ARRAY_PTR(p) { if ((p) != NULL) { delete[] (p); (p) = NULL; } }

#endif /// _BASE_MACRO_H_
