#include "chef_strings_op.hpp"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <curl/curl.h>

namespace chef {

#define RELEASE_CURL_IF_NEEDED(x) do { \
  if (x != NULL) { \
    curl_easy_cleanup(x); \
    x = NULL; \
  } \
} while(0);

#define RELEASE_CURL_SLIST_IF_NEEDED(x) do { \
  if (x != NULL) { \
    curl_slist_free_all(x); \
    x = NULL; \
  } \
} while(0);

namespace inner {

  inline static size_t response_body_cb(void *contents, size_t size, size_t nmemb, void *userp) {
    size_t realsize = size * nmemb;
    std::string *body = static_cast<std::string *>(userp);
    body->append(static_cast<char *>(contents), realsize);
    return realsize;
  }

  inline static size_t response_header_cb(void *contents, size_t size, size_t nmemb, void *userp) {
    size_t realsize = size * nmemb;
    std::string *headers = static_cast<std::string *>(userp);
    headers->append(static_cast<char *>(contents), realsize);
    return realsize;
  }

} // namespace inner

  inline int http_op::global_init_curl() { return curl_global_init(CURL_GLOBAL_ALL) == CURLE_OK ? 0 : -1; }

  inline void http_op::global_cleanup_curl() { curl_global_cleanup(); }

  inline int http_op::get_or_post(const std::string &url,
                           const std::map<std::string, std::string> *headers,
                           const std::map<std::string, std::string> *cookies,
                           const char *post_data,
                           int timeout_ms,
                           response &resp
  ) {
    CURL *curl = curl_easy_init();
    if (curl == NULL) {
      return -1;
    }

    /// 转换request headers kv至libcurl格式
    curl_slist *request_header_list = NULL;
    if (headers != NULL && !headers->empty()) {
      std::map<std::string, std::string>::const_iterator request_headers_iter = headers->begin();
      for (; request_headers_iter != headers->end(); request_headers_iter++) {
        std::string request_header_line = request_headers_iter->first + ": " + request_headers_iter->second;
        request_header_list = curl_slist_append(request_header_list, request_header_line.c_str());
      }
    }

    /// 转换request cookies kv至libcurl格式
    std::string request_cookies_buf;
    if (cookies != NULL && !cookies->empty()) {
      std::map<std::string, std::string>::const_iterator request_cookies_iter = cookies->begin();
      for (; request_cookies_iter != cookies->end(); request_cookies_iter++) {
        request_cookies_buf += request_cookies_iter->first + ":" + request_cookies_iter->second + ";";
      }
    }

    /// 设置请求地址
    curl_easy_setopt(curl, CURLOPT_URL, url.c_str());

    /// 设置request headers
    if (request_header_list != NULL) {
      curl_easy_setopt(curl, CURLOPT_HTTPHEADER, request_header_list);
    }
    /// 设置request cookies
    if (!request_cookies_buf.empty()) {
      curl_easy_setopt(curl, CURLOPT_COOKIE, request_cookies_buf.c_str());
    }

    /// 设置post data
    if (post_data != NULL) {
      curl_easy_setopt(curl, CURLOPT_COPYPOSTFIELDS, post_data);
    }

    /// 设置请求超时时间，单位毫秒
    curl_easy_setopt(curl, CURLOPT_TIMEOUT_MS, static_cast<long>(timeout_ms));
    /// 屏蔽信号
    curl_easy_setopt(curl, CURLOPT_NOSIGNAL, 1L);
    /// 让libcurl帮忙hold 3xx跳转
    curl_easy_setopt(curl, CURLOPT_FOLLOWLOCATION, 1L);
    /// 设置最大允许跳转的次数，避免无限跳转
    curl_easy_setopt(curl, CURLOPT_MAXREDIRS, 32L);
    /// 打开libcurl的调试输出
    //curl_easy_setopt(curl, CURLOPT_VERBOSE, 1L);

    /// 收body回调
    curl_easy_setopt(curl, CURLOPT_WRITEFUNCTION, inner::response_body_cb);
    curl_easy_setopt(curl, CURLOPT_WRITEDATA, static_cast<void *>(&resp.content_));
    /// 收headers回调
    curl_easy_setopt(curl, CURLOPT_HEADERFUNCTION, inner::response_header_cb);
    std::string resp_headers_buf;
    curl_easy_setopt(curl, CURLOPT_HEADERDATA, static_cast<void *>(&resp_headers_buf));

    /// 开启cookie引擎，收resp cookie
    curl_easy_setopt(curl, CURLOPT_COOKIEFILE, "");

    /// go
    if (curl_easy_perform(curl) != CURLE_OK) {
      RELEASE_CURL_IF_NEEDED(curl);
      RELEASE_CURL_SLIST_IF_NEEDED(request_header_list);
      return -1;
    }

    /// 获取返回的cookie
    struct curl_slist *cs_cookies     = NULL;
    struct curl_slist *cs_cookies_dup = NULL;
    if (curl_easy_getinfo(curl, CURLINFO_COOKIELIST, &cs_cookies) != CURLE_OK) {
      RELEASE_CURL_IF_NEEDED(curl);
      RELEASE_CURL_SLIST_IF_NEEDED(request_header_list);
      return -1;
    }
    cs_cookies_dup = cs_cookies;
    while(cs_cookies_dup) {
      std::vector<std::string> cookie_attrs = strings_op::split_any(std::string(cs_cookies_dup->data), chef::WHITESPACE);
      if (cookie_attrs.size() != 7) {
        continue;
      }
      /// format:
      /// <hostname> \t <include subdomains> \t <path> \t <secure> \t <expiry epoch time> \t <name> \t <value>
      response_cookie rc;
      rc.hostname_                   = cookie_attrs[0];
      rc.is_include_subdomains_      = cookie_attrs[1] == "TRUE" ? true : false;
      rc.path_                       = cookie_attrs[2];
      rc.is_secure_                  = cookie_attrs[3] == "TRUE" ? true : false;
      rc.expiry_epoch_time_          = atol(cookie_attrs[4].c_str());
      rc.name_                       = cookie_attrs[5];
      rc.value_                      = cookie_attrs[6];
      resp.cookies_[cookie_attrs[5]] = rc;
      cs_cookies_dup = cs_cookies_dup->next;
    }
    curl_slist_free_all(cs_cookies);

    /// 获取返回的http code
    curl_easy_getinfo(curl, CURLINFO_RESPONSE_CODE, &resp.code_);

    /// 处理返回的headers
    std::vector<std::string> header_pairs = strings_op::splitlines(resp_headers_buf, false);
    std::vector<std::string>::iterator iter = header_pairs.begin();
    for (; iter != header_pairs.end(); iter++) {
      std::string &head_item = *iter;
      std::size_t split_pos = head_item.find(":");
      if (split_pos == std::string::npos) {
          continue;
      }
      std::string header_k = strings_op::trim_left(strings_op::trim_right(head_item.substr(0, split_pos)));
      std::string header_v = strings_op::trim_left(strings_op::trim_right(head_item.substr(split_pos+1)));
      resp.headers_[header_k] = header_v;
    }

    /// 扫尾
    RELEASE_CURL_IF_NEEDED(curl);
    RELEASE_CURL_SLIST_IF_NEEDED(request_header_list);
    return 0;
  }

#undef RELEASE_CURL_IF_NEEDED
#undef RELEASE_CURL_SLIST_IF_NEEDED

} // namespace chef

