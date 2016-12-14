/**
 * @file   noncopyable.hpp
 * @deps   nope
 *
 * @author
 *   chef <191201771@qq.com>
 *     -initial release xxxx-xx-xx
 *
 * @brief  nope
 *
 */

#ifndef _CHEF_BASE_NONCOPYABLE_H_
#define _CHEF_BASE_NONCOPYABLE_H_

namespace chef {

  class noncopyable {
    public:
      noncopyable() {}
      virtual ~noncopyable() {}

    private:
      noncopyable(const noncopyable &);
      const noncopyable &operator=(const noncopyable &);
  };

  /**
   * 空接口,可起强调作用.
   *
   */
  class copyable {};

} // namespace chef

#endif // _CHEF_BASE_NONCOPYABLE_H_
