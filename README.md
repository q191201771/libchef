# linux c++基础库

### 简介

linux c++后台开发基础库。目标是随时随地都能方便快捷的使用，让后台开发时更关注于服务的业务，逻辑清晰，功能分层。

### 代码结构

```
src/                        ......代码根目录
  base/                     ......基础库
  base_test/                ......基础库相应测试
script/                     ......脚本目录
build/                      ......编译脚本目录
  linux/
  macos/
.gitignore                  ......
LINCENSE                    ......
README.md                   ......
```

### 我的环境

```
ubuntu
Linux ubuntu 4.4.0-62-generic #83-Ubuntu SMP Wed Jan 18 14:10:15 UTC 2017 x86_64 x86_64 x86_64 GNU/Linux
gcc version 5.4.0 20160609 (Ubuntu 5.4.0-6ubuntu1~16.04.4)

macos
Darwin MacdeMacBook-Pro.local 16.5.0 Darwin Kernel Version 16.5.0: Fri Mar  3 16:52:33 PST 2017; root:xnu-3789.51.2~3/RELEASE_X86_64 x86_64
xcode: Version 8.3.2 (8E2002)

windows
window10
visual stdio 2017
```

### 依赖

```
ubuntu
$cd ${ROOT_DIR}/script/ && sh install_ubuntu_deps.sh

macos with xcode
$cd ${ROOT_DIR}/script/ && sh install_macos_deps.sh

NOTICE：${ROOT_DIR} 指git工程根目录，以下所有 ${ROOT_DIR} 都是
```

### 编译

linux下使用scons

```
生成方式：
$cd ${ROOT_DIR}/build/linux
$scons [debug]

输出：
base中各源码对应的编译目标文件.o -> ${ROOT_DIR}/build/linux/output/base/*.o
base_test各源码对应的编译目标文件.o、可执行程序 -> ${ROOT_DIR}/build/linux/output/base_test/

清理：
$cd ${ROOT_DIR}/build/linux && scons -c
```

linux下使用make

```
单纯演示下简单、正确使用Makefile的姿势。条件允许的情况下，应该用更友好些的高级编译工具，比如scons或者CMake。

生成方式：
$cd ${ROOT_DIR}/src/base && make [debug]

输出：
base中各源码对应的依赖关系文件.d、编译目标文件.o、静态库文件libchefbase.a
release:
${ROOT_DIR}/src/base/release/*.d
${ROOT_DIR}/src/base/release/*.o
${ROOT_DIR}/src/base/release/libchefbase.a
debug：
${ROOT_DIR}/src/base/debug/*.d
${ROOT_DIR}/src/base/debug/*.o
${ROOT_DIR}/src/base/debug/libchefbase.a

清理：
$cd ${ROOT_DIR}/src/base && make clean
```

mac

```
用xcode打开 ${ROOT_DIR}/build/macos/starry-night.xcworkspace ，之后就可以愉快的编译、运行、调试啦。
目前只添加了部分文件，因为这些文件在公司的项目中使用了，而有的同事用xcode做开发，所以我至少需要保证它们在xcode下是可编译的。
```

windows

```
玩票性质~
用visual stdio打开 ${ROOT_DIR}/build/windows/starry-night/starry-night.sln
```

### 声明，拷贝自第三方工程的代码

```
src/base/crypto_md5.hpp
https://github.com/zaphoyd/websocketpp/blob/master/websocketpp/common/md5.hpp

src/base/encoding_base64.hpp
https://github.com/zaphoyd/websocketpp/blob/master/websocketpp/base64/base64.hpp
```

