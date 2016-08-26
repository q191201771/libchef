
### c++11以及boost已经能很好hold住，不需要自己实现或过度封装的模块

#### STL

* atomic
* condition_variable
* mutex
* rwlock
* random

#### boost

* asio
* spinlock
* uuid
* object_pool

#### [mysql++](http://tangentsoft.net/mysql++/)

#### [http client](https://github.com/whoshuu/cpr)

### 拷贝自第三方工程的代码

```
src/base/crypto_md5.hpp
https://github.com/zaphoyd/websocketpp/blob/master/websocketpp/common/md5.hpp

src/base/encoding_base64.hpp
https://github.com/zaphoyd/websocketpp/blob/master/websocketpp/base64/base64.hpp
```
