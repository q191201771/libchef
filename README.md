# linux c++基础库

[![Build Status](https://travis-ci.org/q191201771/starry-night.svg?branch=master)](https://travis-ci.org/q191201771/starry-night)
[![License](https://img.shields.io/github/license/mashape/apistatus.svg)](https://github.com/q191201771/starry-night/blob/master/LICENSE)

### 简介

linux c++ server base library.

* header only，直接在你的代码中引入相应的头文件即可使用
* 支持macos和linux两个平台
* 多线程等部分[chef_env.hpp链接]支持c++11和boost切换，所以大部分模块兼容低版本编译器

### 已完成模块

```
chef_buffer.hpp
 * @deps     nope
 * @brief    FIFO的流式buffer，支持自动扩容、收缩，供生产和消费长度不固定的场景使用（例如tcp的读写buffer）

chef_count_dump.hpp
 * @deps     c++11 or libboost
 * @brief
 *   - class multi_tag_counter 在各种线程模型下高效的对多个tag进行计数（打点）
 *   - class multi_tag_count_dumper 支持定时将计数落盘，可通过 $watch -n 1 'cat xxx.txt' 观察打点变化

chef_crypto_md5_op.hpp
 * @deps     nope
 * @brief    md5加密

chef_crypto_sha1_op.hpp
 * @deps     nope
 * @brief    sha1加密

chef_defer.hpp
 * @deps     c++11 or libboost
 * @brief    类似golang defer，支持c goto清理等场景

chef_encoding_base64_op.hpp
 * @deps     nope
 * @brief    base64编码、解码

chef_env.hpp
 * @deps     c++11 or libboost
 * @brief
 *   c++11和libboost功能相同部分的wrapper
 *   通过增加一层接入层，使上层代码仅需通过一个宏开关就可以自由切换使用c++11或libboost

chef_filepath_op.hpp
 * @deps     nope
 * @brief    文件、文件夹常用操作帮助函数集合

chef_hash.hpp
 * @deps     c++11
 * @brief
 *   利用变参模板和std::hash实现的万能哈希
 *   可通过多种类型的多个变量组合生成哈希值

chef_noncopyable.hpp
 * @deps     nope
 * @brief    禁用拷贝构造等函数

chef_strings_op.hpp
 * @deps     nope
 * @brief    std::string常用操作帮助函数集合

chef_task_thread.hpp
 * @deps     c++11 or libboost
 * @brief
 *   开启一个线程，可以往里面添加异步任务（支持延时任务）
 *   任务可以是业务方的任意函数（通过bind/function实现）

chef_thread_pool.hpp
 * @deps     c++11 or libboost
 * @brief
 *   线程池，池中的空闲线程抢占式执行丢入其中的任务
 *   适用于丢入的任务不要求强顺序性执行的场景
 *   任务可以是业务方的任意函数（通过bind/function实现）

chef_wait_event_counter.hpp
 * @deps     c++11 or libboost
 * @brief
 *   阻塞等待1~N个事件发生
 *   也可选择设置超时时间，超时后不再阻塞
 *   使用者不用关心条件变量、锁等线程同步实现的细节
```

### 项目文件说明

```
src/                        ......代码根目录
  base/                     ......基础库代码
  base_test/                ......基础库测试代码
third_party/                ......第三方依赖库
.gitignore                  ......
.travis.yml                 ......
LINCENSE                    ......
README.md                   ......
SConstruct                  ......scons编译脚本
ut.sh                       ......执行测试代码脚本
```

### 我的环境

```
gcc version 4.9.2 20150212 (Red Hat 4.9.2-6) (GCC)
gcc version 5.4.0 20160609 (Ubuntu 5.4.0-6ubuntu1~16.04.4)

OS X EI Capitan 10.11.6 Apple LLVM version 8.0.0 (clang-800.0.42.1)
```

### 编译

```
# linux和macos，需要先安装scons，运行scons即可
$scons
```

### 代码说明

```
所有供业务方使用的代码都在 /src/base 目录下，目前大体分为三部分：

/src/base/[chef_xxx.hpp ...]    ......header only的接口文件，业务方直接包含头文件即可使用，每个头文件头部都有简单的功能说明。

/src/base/.wrapper/             ......一些对第三方代码的封装，由于目前starry-night定位于header only且不依赖第三方，所以暂时
                                      隐藏这部分内容，不直接提供给业务方使用
    chef_http_op.hpp[_impl]     ......对libcurl的封装，同步阻塞式完成http get/post
    chef_log.hpp[_impl]         ......对libboost log的封装，近乎零配置，快速使用
    compress_zlib_op.h[.cc]     ......对zlib压缩、解压缩操作的封装

/src/base/.unfinished/          ......等待被整理的代码
    ...
```

### 声明，拷贝自第三方工程的代码

```
src/base/chef_crypto_md5_op.hpp https://github.com/zaphoyd/websocketpp/blob/master/websocketpp/common/md5.hpp

src/base/chef_crypto_sha1_op.h https://github.com/antirez/redis/releases/tag/3.2.9 中src/sha1.h和src/sha1.c

src/base/chef_encoding_base64_op.hpp https://github.com/zaphoyd/websocketpp/blob/master/websocketpp/base64/base64.hpp
```

