# linux c++自用脚手架

## 代码正在火热更新中

## 部分拷贝自第三方工程的代码

```
src/base/crypto_md5.hpp
https://github.com/zaphoyd/websocketpp/blob/master/websocketpp/common/md5.hpp

src/base/encoding_base64.hpp
https://github.com/zaphoyd/websocketpp/blob/master/websocketpp/base64/base64.hpp
```

## 代码结构

```
build/              ......编译后生成的中间文件，可执行文件
src/                ......代码根目录
    base/           ......基础库
    base_test/      ......基础库相应测试
.gitignore          ......
clean.sh            ......清理编译生成文件的脚本
LINCENSE            ......
README              ......
SConstruct          ......SCons文件
unit_test.sh        ......运行单元测试的脚本
```

## 我的环境

```
Ubuntu 14.04.4 LTS (GNU/Linux 4.2.0-27-generic x86_64)

gcc version 4.8.4

boost 1.54.0 静态库

libjsoncpp.a
```
