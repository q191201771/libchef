#!/usr/bin/env bash
# @file   clean.sh
# @author
#   chef <191201771@qq.com>
#     -created 2017-05-14 10:14:14
#     -initial release xxxx-xx-xx
# @brief
#   清理脚本

. ./common.sh

find ${OUT_DIR} -name '*.gch' | xargs rm
find ${OUT_DIR} -name '*.log.chef' | xargs rm

cd ${SCONS_ROOT_DIR}
scons -c

