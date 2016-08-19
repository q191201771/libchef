# linux c++自用脚手架

## 持续更新中

## 拷贝自第三方库的代码

```
src/base/md5.hpp
https://github.com/zaphoyd/websocketpp/blob/master/websocketpp/common/md5.hpp
```

## 代码结构

```
build/              ......编译后的中间文件，可执行文件
src/                ......代码根目录
    base/           ......基础库
    base_test/      ......基础库测试
.gitignore          ......
clean.sh            ......清理脚本
LINCENSE            ......
README              ......
SConstruct          ......SCons文件
```

## 我的环境

```
Ubuntu 14.04.4 LTS (GNU/Linux 4.2.0-27-generic x86_64)

gcc version 4.8.4

boost 1.54.0 静态库
```
