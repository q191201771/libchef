cmake_minimum_required(VERSION 3.0)

project(starry-night C CXX)

set(CMAKE_CXX_FLAGS
  "-g \
  -std=c++0x \
  -pipe \
  -fPIC \
  -Wall \
  -Wextra \
  -Wunused-parameter \
  -Woverloaded-virtual \
  -Wpointer-arith \
  -Wshadow \
  -Wwrite-strings \
  -Wcast-align"
  )
set(CMAKE_CXX_FLAGS_DEBUG "-O0")
set(CMAKE_CXX_FLAGS_RELEASE "-O2 -DNDEBUG")

if (DARWIN)
elseif (LINUX)
  set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -pthread")
ENDIF ()

set(EXECUTABLE_OUTPUT_PATH ${PROJECT_BINARY_DIR}/bin)

add_subdirectory(src/chef_base_test)
