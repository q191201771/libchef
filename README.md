# linux c++基础库

[![Build Status](https://travis-ci.org/q191201771/starry-night.svg?branch=master)](https://travis-ci.org/q191201771/starry-night)
[![License](https://img.shields.io/github/license/mashape/apistatus.svg)](https://github.com/q191201771/starry-night/blob/master/LICENSE)

### 简介

linux c++后台开发基础库。header only，直接在你的代码中引入相应的头文件即可使用。

### 代码结构

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

