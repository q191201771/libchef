### v1.11.0 (2021/08/07)

- [feat] 新增class be_le_op，处理大小端转换
- [refactor] 删除func chef::stuff_op::read_be_int
- [refactor] 修复编译警告，chef_crypto_hmac_sha256和chef_crypto_sha256_op
- [fix] 函数stringify_stl_one_支持所有整型
- [chore] 修复linux的编译问题
- [test] CI不检查chef::this_machine_op::net_interface_bytes的结果

---

`v1.10.17` `~`

* bugfix：无
* 不兼容性修改：chef::stuff_op::bytes_to_hex 新增参数with_ascii，如果为true，则在序列化二进制流的同时也序列化出可显示的字符串流，默认值为true
* 兼容性修改：新增函数 chef::basic_buffer<>::seek_write_pos_rollback，写入位置可前移用于消除回退部分已写入未读取的内容
* 兼容性修改：部分类禁用的拷贝构造函数的参数加上const修饰
* 新增：无
* 其他：无


`v1.9.17` `~`

* bugfix：macos平台下，函数 chef::this_process_op::user_name 和 chef::this_process_op::boot_timestamp 修改实现
* 不兼容性修改：类 chef::buffer 更名为 chef::basic_buffer，并且变成了模板类，新增两个模板参数表示数据类型和数据长度类型
* 兼容性修改：函数 chef::strings_op::split 新增参数 split_once，为 true 表示只分割一次，参数默认值为false
* 新增：文件 chef_crypto_sha256_op.hpp，用于 sha256 加密
* 新增：文件 chef_crypto_hmac_sha256.hpp，用于 hmac sha256 加密
* 其他：工程名由 starry-night 修改为 libchef
* 其他：README.md 中的文件目录树格式
* 其他：CMake 最小版本依赖由 3.0 变为 2.8

`v1.8.17` `~`

* bugfix：无
* 不兼容性修改：大部分类禁用拷贝构造函数和赋值函数
* 兼容性修改：无
* 新增：函数 chef::strings_op::string_printf -> 类似于c字符串格式化函数snprintf，但不需要对结果的长度做预估，输出结果为std::string类型
* 新增：函数 chef::strings_op::url_encode 和 chef::strings_op::url_decode -> url编码、解码
* 新增：函数 chef::filepath_op::is_abs_path -> 判断路径是否为绝对路径格式（以`/`字符开头）
* 其他：所有源码文件头部说明加入@license字段，内容包含工程的github地址

`v1.7.17` `~`

* bugfix：无
* 不兼容性修改：无
* 兼容性修改：无
* 新增：函数 chef::this_machina_op::host_name -> 获取主机名
* 新增：函数 chef::strings_op::text_flow_wrap -> 将文本中过长的行按固定宽度限制换行
* 其他：包含 inttypes.h 的地方修改为 cinttypes
* 其他：增加 CHANGELOG.md 文件用于记录每次发release版本时的具体修改内容

`v1.7.16` `~`

    bugfix：无
    不兼容性修改：无
    兼容性修改：无
    新增：无
    其他：
      整理源码目录，将库文件源码从 /src/chef_base 移动至 /include/chef_base ，
      将相应的测试文件从 /src/chef_base_test 移动至 /test/chef_base_test

`v1.5.16` `~`

    bugfix：无
    不兼容性修改：无
    兼容性修改：无
    新增：
      - 函数 chef::stuff_op::read_be_int 读取大端2~4字节数值转为int
      - 函数 chef::task_thread::stop_and_join 提供显式接口，停止线程并等待线程结束
    其他：无

`v1.5.15` `~`

    bugfix：无
    不兼容性修改：无
    兼容性修改：无
    新增：
      - 宏 CHEF_PROPERTY_WITH_INIT_VALUE (chef_snippet.hpp) 创建类数据成员时附带初始化值
      - 函数 chef::stuff_op::unix_timestamp_msec 获取当前unix时间戳，单位毫秒
    其他：无

`v1.5.14` `16 Nov 2018`

    bugfix：无
    不兼容性修改：无
    兼容性修改：无
    新增：
      - chef_filelock.hpp 可用于多进程间（无亲缘关系的进程间也可以）的互斥锁，并且
        是进程崩溃安全的（即进程退出时会自动释放持有的文件锁）。
      - chef::stuff::bytes_to_hex 函数，将字节流逐个序列化为16进制FF格式，空格分开，
        <num_per_line>换行，可用于debug显示
    其他：无

`v1.5.13` `13 Nov 2018`

    bugfix：无
    不兼容性修改：无
    兼容性修改：无
    新增：chef_stuff_op.hpp  一些暂时没归类的功能代码片段
    其他：
      - 无用代码部分删除，部分暂时放至.invisible目录下

`v1.5.12` `5 Nov 2018`

    bugfix：
      - chef::this_process_op::num_of_threads 函数在macos下可能解析错误
      - chef_log.hpp 获取可执行程序名称可能出现错误
    不兼容性修改：无
    兼容性修改：无
    新增：chef_daemon_op.hpp 守护进程
    其他：
      - 增加CMake编译方式（scons仍然保留）
      - README.md中增加一些徽章
      - 删除部分不再使用的代码

`v1.4.12` `30 Oct 2018`

    bugfix：无
    不兼容性修改：无
    兼容性修改：无
    新增：chef_skiplist.hpp 跳表
    其他：无

`v1.4.11` `18 Oct 2018`

    bugfix：无
    不兼容性修改：无
    兼容性修改：无
    新增：chef_consistent_hash.hpp 一致性哈希
    其他：无

`v1.4.10` `17 Oct 2018`

    bugfix：无
    不兼容性修改：无
    兼容性修改：无
    新增：
      chef_snippet.hpp
      用宏减少一些手写重复代码。比如你有一个结构体，里面有各种类型的各种名称的成
      员变量，有可能有锁或无锁。你不再需要手写这些变量的声明、set、get函数等一堆代码
    其他：无

`v1.4.9` `17 Oct 2018`

    bugfix：无
    不兼容性修改：无
    兼容性修改：无
    新增：
      chef_weight_selector.hpp
      往容器中添加带有权重的元素，根据权重轮询（Weight Round-Robin）返回容器中的元素
    其他：无

`v1.4.8` `30 Sep 2018`

    bugfix：无
    不兼容性修改：无
    兼容性修改：无
    新增：
      chef_this_process_op.hpp
      获取当前进程维度的信息，比如进程启动时间，当前线程数，分页大小，内存占用，
      可执行文件路径，进程号，用户号，用户名，编译时间git版本等
    其他：
      目录 src/base 修改成 src/chef_base
      目录 src/base_test 修改成 src/chef_base_test

`v1.4.7` `29 Sep 2018`

    bugfix：无
    不兼容性修改：无
    兼容性修改：无
    新增：chef_this_machine_op.hpp 获取机器维度的信息，比如CPU，内存，带宽，开机时间等
    其他：无

`v1.4.6` `21 Sep 2018`

    bugfix：无
    不兼容性修改：
      - chef::os_exec_op::run_command 函数返回命令是否成功执行标志，返回命令退出码
      - chef::Lru类名变更为小写风格chef::lru
    兼容性修改：
      - chef_crypto_md5_op.hpp 使用c++风格类型转换避免编译警告
    新增：无
    其他：无

`v1.3.6` `14 Sep 2018`

    bugfix：无
    不兼容性修改：无
    兼容性修改：无
    新增：
      - chef_strings_op.hpp 添加replace_first和replace_last函数，用于替换字符串
        中第一个或最后一个匹配的子串
    其他：
      - ut.sh 编译和单元测试串行执行，上一步成功后才执行下一步，脚本返回0表示编
        译和所有测试都通过
      - travis-ci 由只编译改成编译后再执行所有单元测试
      - 修复单元测试中关于task_thread::num_of_undone_task函数的测试内容

`v1.3.5` `10 Sep 2018`

    bugfix：无
    不兼容性修改：无
    兼容性修改：chef_count_dump.hpp 可以指定写文件时使用覆盖模式或追加模式（默认依然为覆盖）
    新增：无
    其他：
      - macos下移除链接选项 -pthread -static-libstdc++
      - 大部分头文件头部添加使用示例

`v1.3.4` `10 Sep 2018`

    bugfix：无
    不兼容性修改：无
    兼容性修改：无
    新增：chef_fmt_op.hpp 方便的生成格式化字符串，类似于sprintf，格式符由`%d` `%s`等等简化成统一的`{}`
    其他：无

`v1.3.3` `28 Aug 2018`

    bugfix：无
    不兼容性修改：无
    兼容性修改：无
    新增：无
    其他：增加pretag.sh脚本，以后每次打发版tag都会将tag号写入头文件的头部说明中

`v1.3.2` `25 Aug 2018`

    bugfix：无
    不兼容性修改：
      - chef_stringify_stl.hpp
        - 样式相关的多个参数聚合成结构体参数
    兼容性修改：无
    新增：无
    其他：头文件头部增加使用说明

`v1.2.2` `18 Aug 2018`

    bugfix：无
    不兼容性修改：
      - chef_stringify_stl.hpp
        - 所有容器类型的字符串化函数全部使用重载的函数名stringify_stl
        - 支持多维嵌套容器
    兼容性修改：无
    新增：无
    其他：无

`v1.1.2` `7 Aug 2018`

    bugfix：无
    不兼容性修改：无
    兼容性修改：
      - chef_stringify_stl.hpp添加对stl容器stack,queue,priority_queue的支持
    新增：无
    其他：无

`v1.1.1` `3 Aug 2018`

    bugfix：无
    不兼容性修改：无
    兼容性修改：
      - 如果使用了chef_env.hpp，却没有使用c++11以上编译器编译且没有定义
        CHEF_USE_BOOST宏表明使用libboost替代，则给出编译错误
    新增：无
    其他：无

`v1.1.0` `12 Jul 2018`

    bugfix：无
    不兼容性修改：无
    兼容性修改：无
    新增：无
    其他：
      - 版本号前缀由release-改为v
      - README.d中增加badge
