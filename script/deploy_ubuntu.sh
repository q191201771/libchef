#!/usr/bin/env bash
# @file   deploy_ubuntu.sh
# @author
#   chef <191201771@qq.com>
#     -created 2017-05-14 10:19:33
#     -initial release xxxx-xx-xx
# @brief
#   把工程同步至ubuntu上

. ./common.sh

sh clean.sh
rsync -rlpgoDvP -e 'ssh -p 4022' --progress ${ROOT_DIR} --exclude-from=exclude_list chef@localhost:/tmp/starry-night
ssh -p 4022 chef@localhost "cd /tmp/starry-night/build/linux && scons"
#ssh -p 4022 chef@localhost "cd /tmp/starry-night/build/linux && scons mode=debug"
#ssh -p 4022 chef@localhost "cd /tmp/starry-night/build/linux && scons mode=debug && cd /tmp/starry-night/script && ./unit_test.sh"
#ssh -p 4022 chef@localhost "cd /tmp/starry-night/src/base && make clean && make"
