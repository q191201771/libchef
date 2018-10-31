#!/bin/bash

set -x

# config
CHEF_BUILD_TOOL=${CHEF_BUILD_TOOL:-cmake}   # option: "cmake" "scons"
CHEF_BUILD_TYPE=${CHEF_BUILD_TYPE:-release} # option: "release" "debug"

ROOT_DIR=`pwd`

if [[ ${CHEF_BUILD_TOOL} == 'scons' ]]; then
    scons mode=$CHEF_BUILD_TYPE -j8
elif [[ ${CHEF_BUILD_TOOL} == 'cmake' ]]; then
    mkdir -p output && \
      cd output && \
      cmake -DCMAKE_BUILD_TYPE=$BUILD_TYPE $ROOT_DIR && \
      make
else
    echo 'Failed since CHEF_BUILD_TOOL only support cmake or scons.'
    exit -1
fi
