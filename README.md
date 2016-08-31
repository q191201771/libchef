# linux c++脚手架

### 代码添加中...

### 代码结构

```
build/              ......编译后生成的中间文件，可执行文件
src/                ......代码根目录
    base/           ......基础库
    base_test/      ......基础库相应测试
.gitignore          ......
clean.sh            ......清理编译生成文件的脚本
LINCENSE            ......
README.md           ......
SConstruct          ......SCons文件
unit_test.sh        ......运行单元测试的脚本
```

### 我的环境

```
Ubuntu 14.04.4 LTS (GNU/Linux 4.2.0-27-generic x86_64)

gcc version 4.8.4

boost 1.54.0 静态库

libjsoncpp.a
libhiredis.a
```

### c++11以及boost已经能很好hold住，不需要自己实现或过度封装的模块

#### STL

* atomic
* condition_variable
* mutex
* rwlock
* random

#### boost

* spinlock
* uuid
* object_pool

#### boost asio [自己写的demo](https://github.com/q191201771/asio-tutorial)

#### mysql++ [官方地址](http://tangentsoft.net/mysql++/)

#### http client [cpr github地址](https://github.com/whoshuu/cpr)

### 拷贝自第三方工程的代码

```
src/base/crypto_md5.hpp
https://github.com/zaphoyd/websocketpp/blob/master/websocketpp/common/md5.hpp

src/base/encoding_base64.hpp
https://github.com/zaphoyd/websocketpp/blob/master/websocketpp/base64/base64.hpp
```
