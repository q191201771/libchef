# linux c++脚手架

### 代码结构

```
src/                        ......代码根目录
  base/                     ......基础库
  base_test/                ......基础库相应测试
  recipe/                   ......平时一些小demo
build/                      ......编译后生成的中间文件，可执行文件
.gitignore                  ......
LINCENSE                    ......
README.md                   ......
SConstruct                  ......SCons文件
clean.sh                    ......清理编译生成文件的脚本
./deploy_virtual_box.sh     ......发布至本地ubuntu虚拟机编译跑单元测试
unit_test.sh                ......运行单元测试的脚本
```

### 质量保证

* 全测试单元覆盖
* valgrind clean

### 我的环境

```
Ubuntu 14.04.4 LTS (GNU/Linux 4.2.0-27-generic x86_64)
gcc version 4.8.4
boost 1.54.0 静态库
或
Ubuntu 16.04.1 LTS (GNU/Linux 4.4.0-34-generic x86_64)
gcc version 5.4.0
boost 1.58.0 静态库

libjsoncpp.a
libhiredis.a

```

### 编译

```
$sudo apt-get install scons g++ libjsoncpp-dev libboost-dev libboost-all-dev libhiredis-dev -y
$scons
```

### c++11 or boost

* thread
* mutex
* spinlock
* rwlock
* lock_guard
* condition_variable
* this_thread
* smart_ptr
* bind/function/ref
* chrono
* atomic
* random
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

### 代码还在持续添加中...
