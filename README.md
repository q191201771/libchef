# linux c++脚手架

### 代码结构

```
src/                        ......代码根目录
  base/                     ......基础库
  base_test/                ......基础库相应测试
build/                      ......编译后生成的中间文件，可执行文件
script/                     ......脚本目录
SConstruct                  ......SCons文件
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

```
$scons
```

### 声明，拷贝自第三方工程的代码

```
src/base/crypto_md5.hpp
https://github.com/zaphoyd/websocketpp/blob/master/websocketpp/common/md5.hpp

src/base/encoding_base64.hpp
https://github.com/zaphoyd/websocketpp/blob/master/websocketpp/base64/base64.hpp
```

