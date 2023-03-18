cmake_minimum_required(VERSION 3.9)
set(CMAKE_CXX_STANDARD 14)
project(shm-ipc)

# share memory lib
include_directories(
        include
        3rdparty
        src
        /usr/local/lib/python3.6/dist-packages/numpy/core/include
        /usr/include/python3.6)

if(UNIX)
    file(GLOB_RECURSE SHM_SRC_FILES src/libipc/platform/*_linux.cpp)
    # ${SHM_DIR}/demo/PyCpp/Client.cpp
    # ${SHM_DIR}/demo/PyCpp/util.cpp
else()
    file(GLOB_RECURSE SHM_SRC_FILES src/libipc/platform/*_win.cpp)
endif()
aux_source_directory(src SHM_SRC_FILES)
file(GLOB_RECURSE SHM_HEAD_FILES
        include/libipc/*.h
        src/libipc/*.h
        src/libipc/*.inc
        src/libipc/circ/*.h
        src/libipc/memory/*.h
        src/libipc/platform/*.h
        src/libipc/utility/*.h
        src/demo/PyCpp/*.h)

ADD_LIBRARY(shm-ipc ${SHM_SRC_FILES} ${SHM_HEAD_FILES})
