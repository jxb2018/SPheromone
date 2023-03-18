cmake_minimum_required(VERSION 3.6)
project(SPheromone)
set(CMAKE_CXX_STANDARD 17)

add_compile_options(-DCPPZMQ_BUILD_TESTS=OFF)

set(BASE_DIR ${CMAKE_SOURCE_DIR})

# header
include_directories(deps/shm-ipc/include)
include_directories(include)
include_directories(common/include)
include_directories(src-gen/)

# deps
add_subdirectory(deps/shm-ipc shm-ipc)
add_subdirectory(deps/spdlog spdlog)
add_subdirectory(deps/yaml-cpp yaml-cpp)
add_subdirectory(deps/zeromq zeromq)
add_subdirectory(deps/fmt fmt)

## zeromq
set(CPPZMQ_BUILD_TESTS FALSE)
set(ZeroMQ_DIR ${CMAKE_CURRENT_BINARY_DIR}/deps/zeromq)
add_subdirectory(deps/cppzmq cppzmq)

## protobuf
INCLUDE(FindProtobuf)
FIND_PACKAGE(Protobuf REQUIRED)
INCLUDE_DIRECTORIES(${PROTOBUF_INCLUDE_DIR})

# gen src and header
#PROTOBUF_GENERATE_CPP(PROTO_SRCS PROTO_HDRS
#        common/proto/anna.proto
#        common/proto/common.proto
#        common/proto/kvs.proto
#        common/proto/operation.proto)
#add_library(faas-proto ${PROTO_SRCS} ${PROTO_HDRS})
#include_directories(${PROTO_HDRS})
#include_directories(${CMAKE_CURRENT_BINARY_DIR})

## submodule
add_subdirectory(src/executor executor)
add_subdirectory(src/manager manager)
add_subdirectory(src/coordinator coordinator)