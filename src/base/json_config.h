/**
 * @file   json_config.h/json_config.cc
 * @deps   jsoncpp
 *
 * @author
 *   chef <191201771@qq.com>
 *     -initial release 2016-08-31
 *
 * @brief  读取json类型配置文件，映射到类中。
 *
 */

#ifndef _CHEF_BASE_JSON_CONFIG_H_
#define _CHEF_BASE_JSON_CONFIG_H_

#include "noncopyable.hpp"
#include <json/json.h>
#include <string>

namespace chef {

  class json_config : chef::noncopyable {
    public:
      json_config();
      virtual ~json_config();

      /**
       * 加载配置文件
       *
       * @return
       *   0  成功
       *   -1 失败(打开文件失败 || 文件内容不是json格式)
       *
       */
      int load_file(const std::string &filename);

      /**
       * 反序列化配置内容至私有数据成员中
       * 实现中请使用try catch捕获错误
       *
       * @return 0 成功 -1 失败
       *
       */
      virtual int unmarshal() = 0;

      /**
       * @return 完整配置文件内容(如果未加载或加载配置失败，则返回空)
       *
       */
      std::string styled_string() const { return styled_string_; }

    protected:
      Json::Value root_;

    private:
      std::string styled_string_;
  };

} // namespace chef

#endif // _CHEF_BASE_JSON_CONFIG_H_
