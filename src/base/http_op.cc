#include "http_op.h"
#include "strings_op.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <curl/curl.h>

namespace inner {

  static size_t response_body_cb(void *contents, size_t size, size_t nmemb, void *userp) {
    size_t realsize = size * nmemb;
    std::string *body = static_cast<std::string *>(userp);
    body->append(static_cast<char *>(contents), realsize);
    return realsize;
  }

  static size_t response_header_cb(void *contents, size_t size, size_t nmemb, void *userp) {
    size_t realsize = size * nmemb;
    std::string *headers = static_cast<std::string *>(userp);
    headers->append(static_cast<char *>(contents), realsize);
    return realsize;
  }

} // namespace inner

namespace chef {

  int http_op::global_init_curl() { return curl_global_init(CURL_GLOBAL_ALL) == CURLE_OK ? 0 : -1; }

  void http_op::global_cleanup_curl() { curl_global_cleanup(); }

  int http_op::get(const std::string &url, const std::map<std::string, std::string> *headers, int timeout_ms, response &resp) {
    CURL *curl = curl_easy_init();
    if (curl == NULL) {
      return -1;
    }

    curl_slist *request_header_list = NULL;
    if (headers != NULL && !headers->empty()) {
      std::map<std::string, std::string>::const_iterator request_headers_iter = headers->begin();
      for (; request_headers_iter != headers->end(); request_headers_iter++) {
        std::string request_header_line = request_headers_iter->first+": "+request_headers_iter->second;
        request_header_list = curl_slist_append(request_header_list, request_header_line.c_str());
      }
    }


    std::string resp_headers_buf;

    curl_easy_setopt(curl, CURLOPT_URL, url.c_str());
    if (request_header_list) {
      curl_easy_setopt(curl, CURLOPT_HTTPHEADER, request_header_list);
    }

    curl_easy_setopt(curl, CURLOPT_TIMEOUT_MS, static_cast<long>(timeout_ms));
    curl_easy_setopt(curl, CURLOPT_NOSIGNAL, 1L);
    //curl_easy_setopt(curl, CURLOPT_VERBOSE, 1L);

    curl_easy_setopt(curl, CURLOPT_WRITEFUNCTION, inner::response_body_cb);
    curl_easy_setopt(curl, CURLOPT_WRITEDATA, static_cast<void *>(&resp.content_));
    curl_easy_setopt(curl, CURLOPT_HEADERFUNCTION, inner::response_header_cb);
    curl_easy_setopt(curl, CURLOPT_HEADERDATA, static_cast<void *>(&resp_headers_buf));

    if (curl_easy_perform(curl) != CURLE_OK) {
      return -1;
    }

    curl_easy_getinfo(curl, CURLINFO_RESPONSE_CODE, &resp.code_);

    std::vector<std::string> header_pairs = strings_op::splitlines(resp_headers_buf, false);
    std::vector<std::string>::iterator iter = header_pairs.begin();
    for (; iter != header_pairs.end(); iter++) {
      std::vector<std::string> header_kv = strings_op::split(*iter, ':', true);
      if (header_kv.size() < 2 || strings_op::contains(header_kv[0], "Set-Cookie")) {
        printf("fucked,%s\n", iter->c_str());
        continue;
      }
      std::string header_k = strings_op::trim_left(strings_op::trim_right(header_kv[0]));
      std::string header_v = header_kv[1];

      /// TODO 实际上，这里我们只需要split第一个':'，偷懒用了strings_op，就再把后面多切的join一下好了，反正header不会很大，
      /// 做个效率和可读的trade off~
      /// 后面可以考虑不用strings_op或给strings_op::split添加一个只切一次的参数。。
      if (header_kv.size() > 2) {
        header_v = strings_op::join(std::vector<std::string>(header_kv.begin()+1, header_kv.end()), ":");
      }

      resp.headers_[header_k] = strings_op::trim_left(strings_op::trim_right(header_v));
    }

    curl_easy_cleanup(curl);
    if (request_header_list != NULL) {
      curl_slist_free_all(request_header_list);
    }
    return 0;
  }

} // namespace chef


