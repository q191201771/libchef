# linux c++基础库

### 简介

linux c++后台开发基础库。

### 代码结构

```
src/                        ......代码根目录
  base/                     ......基础库
  base_test/                ......基础库相应测试
.gitignore                  ......
LINCENSE                    ......
README.md                   ......
```

### 我的环境

```
gcc version 4.9.2 20150212 (Red Hat 4.9.2-6) (GCC)
gcc version 5.4.0 20160609 (Ubuntu 5.4.0-6ubuntu1~16.04.4)

OS X EI Capitan 10.11.6 Apple LLVM version 8.0.0 (clang-800.0.42.1)
```

### 依赖

* libcurl
  * chef_http_op.h[.cc]
* boost 1.66
  * chef_log.h[.cc]

### 编译

```
# linux和macos,需要先安装scons,然后直接运行scons
$scons
# 如果想要编译debug版本
$scons mode=debug
```

### 声明，拷贝自第三方工程的代码

```
src/base/chef_crypto_md5_op.h https://github.com/zaphoyd/websocketpp/blob/master/websocketpp/common/md5.hpp

src/base/chef_encoding_base64_op.h https://github.com/zaphoyd/websocketpp/blob/master/websocketpp/base64/base64.hpp

src/base/crypto_sha1_op.h https://github.com/antirez/redis/releases/tag/3.2.9 中src/sha1.h和src/sha1.c
```

