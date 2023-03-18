cmake_minimum_required(VERSION 3.6)
project(SPheromone)
set(CMAKE_CXX_STANDARD 14)

add_compile_options(-DCPPZMQ_BUILD_TESTS=OFF)

# header
include_directories(deps/shm-ipc/include)
include_directories(include)
include_directories(src-gen/)
include_directories(common/include)

# deps
add_subdirectory(deps/shm-ipc shm-ipc)
add_subdirectory(deps/spdlog spdlog)
add_subdirectory(deps/yaml-cpp yaml-cpp)
add_subdirectory(deps/zeromq zeromq)
add_subdirectory(deps/fmt fmt)

set(CPPZMQ_BUILD_TESTS FALSE)
set(ZeroMQ_DIR ${CMAKE_CURRENT_BINARY_DIR}/deps/zeromq)
add_subdirectory(deps/cppzmq cppzmq)

# submodule
add_subdirectory(src/executor executor)