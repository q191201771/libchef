#!/usr/bin/env bash
# @file   install_ubuntu_deps.sh
# @author
#   chef <191201771@qq.com>
#     -created 2017-05-14 11:11:46
#     -initial release xxxx-xx-xx
# @brief
#   安装ubuntu下编译工具、第三方依赖库

sudo apt-get install make g++ scons -y
sudo apt-get install gdb valgrind -y
sudo apt-get install libboost-dev libboost-all-dev -y
sudo apt-get install zlib1g-dev libjsoncpp-dev libhiredis-dev libcurl4-openssl-dev -y
