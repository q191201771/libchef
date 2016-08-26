/**
 * @file   noncopyable.hpp
 * @deps   jsoncpp
 *
 * @author
 *   chef <191201771@qq.com>
 *     -initial release xxxx-xx-xx
 *
 * @brief  读取json类型配置文件，映射到类中。
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
