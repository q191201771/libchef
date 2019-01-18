# libchef - c++ header-only basic library

![platform](https://img.shields.io/badge/platform-linux%20%7C%20macos%20-green.svg)
[![Release](https://img.shields.io/github/release/q191201771/libchef.svg)](https://github.com/q191201771/libchef/releases)
[![Build Status](https://travis-ci.org/q191201771/libchef.svg?branch=master)](https://travis-ci.org/q191201771/libchef)
[![Codacy Badge](https://api.codacy.com/project/badge/Grade/ff1659c0c9b544fba4eab876900b2421)](https://app.codacy.com/app/q191201771/libchef?utm_source=github.com&utm_medium=referral&utm_content=q191201771/libchef&utm_campaign=Badge_Grade_Dashboard)
![stability-experimental](https://img.shields.io/badge/stability-experimental-orange.svg)
[![License](https://img.shields.io/badge/license-MIT-brightgreen.svg)](https://github.com/q191201771/libchef/blob/master/LICENSE)
[![GitHub last commit](https://img.shields.io/github/last-commit/q191201771/libchef.svg)](https://github.com/q191201771/libchef/graphs/commit-activity)
![GitHub code size in bytes](https://img.shields.io/github/languages/code-size/q191201771/libchef.svg)
![GitHub language count](https://img.shields.io/github/languages/count/q191201771/libchef.svg)
![GitHub top language](https://img.shields.io/github/languages/top/q191201771/libchef.svg)
[![PRs Welcome](https://img.shields.io/badge/PRs-welcome-brightgreen.svg)](https://github.com/q191201771/libchef/pulls)

header-only，易于接入。尽可能一个hpp一个模块，需要使用哪个模块提供的功能只需要包含那个模块的头文件即可直接使用，不需要对库单独编译和链接。（目标是像机器猫的万能口袋一样，什么都有，需要什么掏什么，不强制相互捆绑，非全家桶类型）。
支持linux和macos双平台。

## 已完成模块说明

下表中**分类**这一列（只是在README文档中做的功能上的逻辑划分，代码层面和使用上没有分类和区别）：

* `DSAA`: Data Structure And Algorithm（数据结构和算法）
* `MT`: Multi-Thread（多线程相关）
* `IPC`: Inter-Process Communication（多进程相关）
* `SO`: String Operator（字符串操作）
* `SS`: System Stuff（系统编程相关）
* `PH`: Project Helper（工程相关）
* `CE`: Crypto & Encoding（加解密、编解码）

下表中**依赖**这一列：

* `nope`表示无任何依赖
* `chef_env`表示需要使用c++11或libboost(即低版本编译器可使用libboost做兼容，具体见 [chef_env.hpp](https://github.com/q191201771/libchef/blob/master/include/chef_base/chef_env.hpp))
* `c++11`表示必须依赖c++11


分类 | 模块名称 | 依赖 | 功能描述 |
-----|---------|------|----------|
DSAA | chef_consistent_hash.hpp | c++11 | 一致性哈希 |
DSAA | chef_skiplist.hpp        | c++11 | 跳表 |
DSAA | chef_lru.hpp             | nope  | 固定大小的LRU cache，支持插入，查询，以及获取全量列表 |
DSAA | chef_buffer.hpp          | nope  | FIFO的流式buffer，支持自动扩容、收缩，供生产和消费长度不固定的场景使用（例如tcp的读写buffer） |
DSAA | chef_weight_selector.hpp | nope  | 往容器中添加带有权重的元素，根据权重轮询（Weight Round-Robin）返回容器中的元素 |
DSAA | chef_hash.hpp            | c++11 | 利用变参模板和std::hash实现的万能哈希。可通过多种类型的多个变量组合生成哈希值 |
MT | chef_env.hpp                | chef_env | c++11和libboost功能相同部分的wrapper。通过增加一层接入层，使上层代码仅需通过一个宏开关就可以自由切换使用c++11或libboost |
MT | chef_wait_event_counter.hpp | chef_env | 阻塞等待1~N个事件发生。也可选择设置超时时间，超时后不再阻塞。使用者不用关心条件变量、锁等线程同步实现的细节（例如事件发送与接收的时顺，原子计数，虚假唤醒等） |
MT | chef_task_thread.hpp        | chef_env | 开启一个线程，可以往里面持续添加异步任务，任务串行执行，且执行顺序与添加顺序一致。支持添加延时任务。任务可以是业务方的任意函数（通过bind/function实现）。 |
MT | chef_thread_pool.hpp        | chef_env | 线程池，池中的空闲线程抢占式执行加入的任务。适用于任务不要求强顺序性执行的场景。 |
MT | chef_thread_group.hpp       | chef_env | 线程池，添加异步任务时支持 指定线程号 和 池内自动调度 两种方式。指定线程号方式将任务按业务方自身需求分类打到固定线程中执行。池内自动调度方式支持round-robin轮转循环，随机，当前最空闲（即未完成任务数最少）线程三种方式。 |
IPC | chef_filelock.hpp | nope | 文件锁，可用于多进程间（无亲缘关系的进程间也可以）的互斥锁，并且是进程崩溃安全的（即进程退出时会自动释放持有的文件锁）。 |
SO | chef_fmt_op.hpp        | c++11 | 方便的生成格式化字符串，类似于sprintf，格式符由`%d` `%s`等等简化成统一的`{}` |
SO | chef_stringify_stl.hpp | nope  | 字符串化stl容器。支持所有stl类型容器，支持多维嵌套容器，支持容器元素为自定义类型，支持自定义样式 |
SO | chef_strings_op.hpp    | nope | 字符串常用操作帮助函数集合 |
SS | chef_os_exec_op.hpp      | nope | 开启子进程执行shell命令，并阻塞式等待结果 |
SS | chef_this_machine_op.hpp | nope | 获取机器维度的信息，比如CPU，内存，带宽，开机时间等 |
SS | chef_this_process_op.hpp | nope | 获取当前进程维度的信息，比如进程启动时间，当前线程数，分页大小，内存占用，可执行文件路径，进程号，用户号，用户名，编译时间git版本等 |
SS | chef_filepath_op.hpp     | nope | 文件、文件夹常用操作帮助函数集合 |
SS | chef_daemon_op.hpp       | nope | 守护进程 |
SS | chef_env_var_op.hpp      | nope | 读写系统环境变量 |
PH | chef_defer.hpp       | chef_env | 类似golang defer，支持c goto清理等场景 |
PH | chef_count_dump.hpp  | chef_env | 在各种线程模型下高效的对多个tag进行计数（打点）。并支持定时将计数落盘 |
PH | chef_snippet.hpp     | c++11    | 用宏减少一些手写重复代码。比如你有一个结构体，里面有各种类型的各种名称的成员变量，有可能有锁或无锁。你不再需要手写这些变量的声明、set、get函数等一堆代码 |
PH | chef_noncopyable.hpp | nope     | 禁用拷贝构造等函数 |
PH | chef_stuff_op.hpp    | nope     | 一些暂时没归类的功能代码片段 |
CE | chef_crypto_md5_op.hpp        | nope | md5加密 |
CE | chef_crypto_sha1_op.hpp       | nope | sha1加密 |
CE | chef_crypto_sha256_op.hpp     | nope | sha256加密 |
CE | chef_crypto_hmac_sha256.hpp   | nope | hmac sha256加密 |
CE | chef_encoding_base64_op.hpp   | nope | base64编码、解码 |

## 声明，内部实现拷贝自第三方工程的代码

```
include/chef_base/chef_crypto_md5_op.hpp      https://github.com/zaphoyd/websocketpp/blob/master/websocketpp/common/md5.hpp
include/chef_base/chef_crypto_sha1_op.hpp     https://github.com/antirez/redis/blob/unstable/src/sha1.h[.c]
include/chef/base/chef_crypto_sha256_op.hpp   https://github.com/lyokato/cpp-cryptlite/blob/master/include/cryptlite/sha256.h
include/chef/base/chef_crypto_hmac_sha256.hpp https://github.com/lyokato/cpp-cryptlite/blob/master/include/cryptlite/hmac.h
include/chef_base/chef_encoding_base64_op.hpp https://github.com/zaphoyd/websocketpp/blob/master/websocketpp/base64/base64.hpp
```

## 我的环境

```
gcc version 4.9.2 20150212 (Red Hat 4.9.2-6) (GCC)
gcc version 5.4.0 20160609 (Ubuntu 5.4.0-6ubuntu1~16.04.4)

OS X EI Capitan 10.11.6 Apple LLVM version 8.0.0 (clang-800.0.42.1)
```

## 编译

在macos和linux下均支持编译工具scons或者cmake

```
# 使用scons编译
export CHEF_BUILD_TOOL=scons && export CHEF_BUILD_TYPE=Release && ./build.sh
# 使用cmake编译
export CHEF_BUILD_TOOL=cmake && export CHEF_BUILD_TYPE=Release && ./build.sh
# CHEF_BUILD_TYPE=Debug则编译debug版本
```

## 其他

欢迎watch、star、fork，下面是我的微信二维码（微信账号： q191201771），欢迎交流

![微信二维码](https://github.com/q191201771/libchef/blob/master/img/wechat_chef.jpeg?raw=true)

## 项目文件树

```
libchef/                                    ......github根目录
|-- include/                                ......libchef库代码目录
    |-- chef_base/                          ......
        |-- [chef_xxx.hpp ...]              ......核心功能代码模块
        |-- .invisible/                     ......暂时不对外可见的代码
            |-- .wrapper/                   ......一些对第三方代码的封装，由于目前libchef定位于header-only且不依赖第三方，所以暂时隐藏这部分内容，不提供给业务方使用
                |-- chef_http_op.hpp[_impl] ......对libcurl的封装，同步阻塞式完成http get/post
                |-- chef_log.hpp[_impl]     ......对libboost log的封装，近乎零配置，快速使用
                |-- compress_zlib_op.h[.cc] ......对zlib压缩、解压缩操作的封装
            |-- .deprecated/                ......已废弃的代码
|-- test/                                   ......基础库测试代码
    |-- chef_base_test/                     ......
|-- img/                                    ......图片目录
|-- output/                                 ......编译输出文件目录
|-- third_party/                            ......第三方依赖库，目前无第三方依赖
|-- .gitignore                              ......
|-- .travis.yml                             ......
|-- build.sh                                ......编译脚本
|-- clean.sh                                ......清除脚本
|-- CMakeLists.txt                          ......cmake编译脚本
|-- LINCENSE                                ......
|-- pretag.sh                               ......打git tag脚本
|-- SConstruct                              ......scons编译脚本
|-- ut.sh                                   ......执行测试代码脚本
|-- README.md                               ......
|-- SECRET.md                               ......
```

