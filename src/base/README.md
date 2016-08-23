## base

#### c++11以及boost已经能很好hold住，不需要自己实现或过度封装的模块

```
atomic
condition_variable
mutex
rwlock
random

signal
spinlock
uuid
object_pool
```

#### 拷贝自第三方工程的代码

```
src/base/crypto_md5.hpp
https://github.com/zaphoyd/websocketpp/blob/master/websocketpp/common/md5.hpp

src/base/encoding_base64.hpp
https://github.com/zaphoyd/websocketpp/blob/master/websocketpp/base64/base64.hpp
```
