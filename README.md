# linux c++脚手架

### 代码结构

```
src/                        ......代码根目录
  base/                     ......基础库
  base_test/                ......基础库相应测试
script/                     ......脚本目录
output/                     ......编译后生成的中间文件，可执行文件
SConstruct                  ......SCons文件，用于编译
.gitignore                  ......
LINCENSE                    ......
README.md                   ......
```

### 我的环境

```
ubuntu-16.04.2-server-amd64
Linux ubuntu 4.4.0-62-generic #83-Ubuntu SMP Wed Jan 18 14:10:15 UTC 2017 x86_64 x86_64 x86_64 GNU/Linux
gcc version 5.4.0 20160609 (Ubuntu 5.4.0-6ubuntu1~16.04.4)
```

### 编译

linux下使用scons

```
生成方式：
$cd ${ROOT_DIR}
$scons
如果想生成debug版本，可以
$scons mode=debug

输出：
base中各源码对应的编译目标文件
{ROOT_DIR}/output/base/*.o
base_test各源码对应的编译目标文件.d、可执行程序
{ROOT_DIR}/output/base_test/

清理：
$cd ${ROOT_DIR}
$scons -c
```

linux下使用make

```
生成方式：
$cd ${ROOT_DIR}/src/base
$make
如果想生成debug版本，可以
$make debug

输出：
base中各源码对应的各源码依赖关系文件.d文件、编译目标文件.d、静态库文件libchefbase.a
release:
{ROOT_DIR}/src/base/release/*.d
{ROOT_DIR}/src/base/release/*.o
{ROOT_DIR}/src/base/release/libchefbase.a
debug：
{ROOT_DIR}/src/base/debug/*.d
{ROOT_DIR}/src/base/debug/*.o
{ROOT_DIR}/src/base/debug/libchefbase.a

清理：
$cd {ROOT_DIR}/src/base
$make clean
```

mac

### 声明，拷贝自第三方工程的代码

```
src/base/crypto_md5.hpp
https://github.com/zaphoyd/websocketpp/blob/master/websocketpp/common/md5.hpp

src/base/encoding_base64.hpp
https://github.com/zaphoyd/websocketpp/blob/master/websocketpp/base64/base64.hpp
```

