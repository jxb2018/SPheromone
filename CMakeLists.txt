cmake_minimum_required(VERSION 3.6)
project(SPheromone)
set(CMAKE_CXX_STANDARD 14)

add_subdirectory(deps/shm-ipc shm-ipc)
add_subdirectory(deps/spdlog spdlog)
add_subdirectory(deps/yaml-cpp yaml-cpp)
add_subdirectory(deps/zeromq zeromq)