cmake_minimum_required(VERSION 2.8)
set(CMAKE_VERBOSE_MAKEFILE ON)

project(libchef C CXX)

set(CMAKE_CXX_FLAGS "-g -pthread -std=c++0x -pipe -fPIC")
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -Wall -Wextra -Wunused-parameter -Woverloaded-virtual -Wpointer-arith -Wshadow -Wwrite-strings -Wcast-align")

set(CMAKE_CXX_FLAGS_DEBUG "-O0")
set(CMAKE_CXX_FLAGS_RELEASE "-O2 -DNDEBUG")

if(APPLE)
  message(STATUS "os: APPLE")
  #set(COMMON_LIBS pthread)
elseif(UNIX)
  message(STATUS "os: UNIX")
  set(COMMON_LIBS pthread rt)
  set(CMAKE_EXE_LINKER_FLAGS "-static-libstdc++")
endif()

set(EXECUTABLE_OUTPUT_PATH ${PROJECT_BINARY_DIR}/bin)

include_directories(include)
add_subdirectory(test/chef_base_test)
