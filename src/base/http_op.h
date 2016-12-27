/**
 * @file   http_op.h/http_op.cc
 * @deps   libcurl/strings_op.h
 *
 * @author
 *   chef <19120177@qq.com>
 *     -initial release xxxx-xx-xx
 *
 * @brief 一些http操作的封装
 *
 */

#ifndef _BASE_HTTP_OP_H_
#define _BASE_HTTP_OP_H_

#include <map>
#include <string>

namespace chef {

  class http_op {
    public:
      /**
       * @breif 内部调用curl_global_init
       *
       * @NOTICE * 根据curl文档上的描述，curl_global_init需要至少调用一次，且不能多线程调用。我本来想自己做个进程级
       *           once封装去调用。后来想想算了，因为应用程序可能在别的地方或库里也会curl_global_init。所以封装一下,
       *           由用户自行保证。
       *         * 增加些许调用方的步骤，保证不出错~
       *
       * @return 0 成功 -1 失败
       *
       */
      static int global_init_curl();

      static void global_cleanup_curl();

    public:

      struct response {
        int                                code_;
        std::string                        content_;
        std::map<std::string, std::string> headers_;
        //std::map<std::string, std::string> cookies_;
      };

      /**
       * @brief http get方法，阻塞直至收到返回或超时~
       *
       * @param        url 'http://'前缀可以有也可以没有，query后缀可以有也可以没有
       * @param    headers 请求时的header组，以kv结构传入，可为空
       * @param timeout_ms 超时时间，单位秒
       * @param   response 返回结果，参见结构体http_op::response
       *
       * @return 0 成功 -1 失败
       *
       */
      static int get(const std::string &url,
                     const std::map<std::string, std::string> headers,
                     int timeout_ms,
                     response &resp /*out*/);

      /// TODO * get request with params,cookies
      ///      * get response with cookies
      ///      * urlencode
      ///      * post
  };

} // namespace inner

#endif // _BASE_HTTP_OP_H_
