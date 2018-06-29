#include "../base/chef_buffer.hpp"
#include <stdio.h>
#include <string.h>
#include <string>
#include "./common/assert_wrapper.hpp"
#include "./common/check_log.hpp"

namespace ut {

class buffer_test {
  public:
    static void test() {
      using chef::buffer;
      buffer buf1;
      assert(buf1.init_capacity_ == 16384);
      assert(buf1.shrink_capacity_ == 1048576);
      assert(buf1.capacity() == 16384);
      assert(buf1.read_index_ == 0);
      assert(buf1.write_index_ == 0);
      char *data = buf1.data_;
      assert(data);
      assert(buf1.read_pos() == data);
      assert(buf1.readable_size() == 0);
      assert(buf1.write_pos() == data);

      buffer buf2(8, 32);
      char *p1 = buf2.read_pos();
      buf2.append("hello world", 5);
      assert(buf2.readable_size() == 5);
      assert(buf2.capacity() == 8);
      assert(memcmp(buf2.read_pos(), "hello", 5) == 0);

      char origin_buf[128] = {0};
      memset(origin_buf, 'x', 127);
      buf2.append(origin_buf, 30);
      char *p2 = buf2.read_pos();
      assert(buf2.readable_size() == 35);
      assert(buf2.capacity() == 64);
      assert(memcmp(buf2.read_pos(), "helloxxx", 8) == 0);
      assert(p1 != p2);

      buf2.erase(32);
      assert(buf2.capacity() == 8);
      char *p3 = buf2.read_pos();
      assert(p3 != p2);
      assert(memcmp(buf2.read_pos(), "xxx", 3) == 0);

      buf2.reserve(10);
      strcpy(buf2.write_pos(), "helloworld");
      buf2.seek_write_pos(10);
      char *p4 = buf2.read_pos();
      assert(p4 != p3);
      assert(buf2.capacity() == 16);
      assert(memcmp(buf2.read_pos(), "xxxhelloworld", 13) == 0);

      buffer buf3(8, 32);
      buf3.append("helloworld", 8);
      assert(buf3.capacity() == 8);

      buffer buf4(8, 32);
      buf4.reserve(16);
      assert(buf4.capacity() == 16);
      memcpy(buf4.write_pos(), "1234567890123456", 16);
      buf4.seek_write_pos(16);

      buffer buf5(buf2);
      assert(buf5.capacity() == 16);
      assert(memcmp(buf5.read_pos(), "xxxhelloworld", 13) == 0);

      buffer buf6 = buf5;
      assert(buf6.read_pos() == buf6.data_);
      assert(memcmp(buf6.read_pos(), "xxxhelloworld", 13) == 0);
      buf6 = buf6;
      assert(memcmp(buf6.read_pos(), "xxxhelloworld", 13) == 0);

      buffer buf7;
      buf7.append("123456789", 10);
      assert(buf7.find("3", 1) == buf7.read_pos() + 2);
      assert(buf7.find("a", 1) == NULL);
      assert(buf7.find("345", 3) == buf7.read_pos() + 2);
      assert(buf7.find("345", 4) == NULL);
      assert(buf7.find("890", 3) == NULL);
      assert(buf7.find("134", 3) == NULL);
      assert(buf7.find_eol() == NULL);
      assert(buf7.find_crlf() == NULL);
      assert(buf7.find('9') == buf7.read_pos() + 8);
      assert(buf7.find('a') == NULL);
      assert(buf7.find("1234567890123", 14) == NULL);
      buf7.erase(buf7.readable_size());
      assert(buf7.readable_size() == 0);
      buf7.append("\0abc\r\n123\r\n4", 12);
      assert(buf7.find("bc\r", 3) == buf7.read_pos() + 2);
      assert(buf7.find('\r') == buf7.read_pos() + 4);
      assert(buf7.find_crlf() == buf7.read_pos() + 4);
      assert(buf7.find_eol() == buf7.read_pos() + 5);
      assert(buf7.find("33", 2) == NULL);

      std::string url = "http://test.www.1234.com/qwer/asdf?zxcv=5678&tyui=ghjk";
      buffer url_buf;
      url_buf.append(url.c_str(), url.size());
      char *pos = url_buf.find("http://", strlen("http://"));
      assert(pos);
      url_buf.erase(strlen("http://"));
      assert(memcmp(url_buf.read_pos(), "test.www.1234.com/qwer/asdf?zxcv=5678&tyui=ghjk",
      strlen("test.www.1234.com/qwer/asdf?zxcv=5678&tyui=ghjk")) == 0);
      pos = url_buf.find("http://", strlen("http://"));
      assert(!pos);
      pos = url_buf.find('/');
      assert(pos);
      std::string domain(url_buf.read_pos(), pos - url_buf.read_pos());
      assert(domain == "test.www.1234.com");
      std::string request(pos, url_buf.write_pos() - pos);
      request = "GET " + request + " /1.0\r\n\r\n";
      assert(request == "GET /qwer/asdf?zxcv=5678&tyui=ghjk /1.0\r\n\r\n");

      std::string response = "HTTP/1.1 200 OK\r\n"
      "Server: nginx/1.6.1\r\n"
      "Date: Fri, 26 Sep 2014 10:00:35 GMT\r\n"
      "Content-Type: application/json\r\n"
      "Connection: close\r\n"
      "X-Powered-By: PHP/5.4.16\r\n"
      "Set-Cookie: PHPSESSID=11415703dt7cn711hl2phc04d3; path=/\r\n"
      "Expires: Thu, 19 Nov 1981 08:52:00 GMT\r\n"
      "Cache-Control: no-store, no-cache, must-revalidate, post-check=0, pre-check=0\r\n"
      "Pragma: no-cache\r\n\r\n"
      "{\"code\":0,\"msg\":\"succ\",\"data\":[],\"ts\":1411725635}";
      buffer response_buf;
      response_buf.append(response.c_str(), response.size());
      /// *
      char *pos1 = response_buf.find_crlf();
      assert(pos1);
      char *pos2 = response_buf.find("200", strlen("200"));
      assert(pos2 && pos2 < pos1);
      response_buf.erase(pos1 - response_buf.read_pos() + 2);
      /// *
      int head_count = 0;
      for (; ; ) {
        pos1 = response_buf.find_crlf();
        assert(pos1);
        pos2 = response_buf.find(':');
        response_buf.erase(pos1 - response_buf.read_pos() + 2);
        if (!pos2 || pos2 - pos1 > 0) {
          break;
        }
        ++head_count;
      }
      assert(head_count == 9);
      /// *
      std::string body(response_buf.read_pos(), response_buf.readable_size());
      assert(body == "{\"code\":0,\"msg\":\"succ\",\"data\":[],\"ts\":1411725635}");

      buffer buf8;
      buf8.append("\v \t  ", 3);
      buf8.trim_left();
      assert(buf8.readable_size() == 0);
      buf8.append("\f  \r\n", 2);
      buf8.trim_right();
      assert(buf8.readable_size() == 0);
      buf8.append(" abc de \v ", 10);
      assert(memcmp(buf8.trim_left(), "abc de \v ", 9) == 0);
      assert(buf8.readable_size() == 9);
      assert(memcmp(buf8.trim_right(), "abc de", 6) == 0);
      assert(buf8.readable_size() == 6);
      char *trim_pos = buf8.read_pos();
      assert(buf8.trim_left() == trim_pos);
      assert(buf8.trim_right() == trim_pos);

      buffer buf("hello world", 11);
      assert(buf.init_capacity_ == 11);
      assert(buf.shrink_capacity_ == 22);
      assert(buf.capacity() == 11);
      assert(buf.readable_size() == 11);
      assert(memcmp(buf.read_pos(), "hello world", 11) == 0);
      buf.clear();
      assert(buf.readable_size() == 0);
      assert(buf.capacity() == 11);
    } // buffer_test::test
}; // class buffer_test

} // ut

int main() {
  ENTER_TEST;

  ut::buffer_test::test();

  return 0;
}
