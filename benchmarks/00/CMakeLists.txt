set(LIBRARY_OUTPUT_PATH /tmp/shm)

include_directories(utils/)

# exp00 frontend
set(EXP00_LIBRARY_FRONTEND exp00_front)
add_library(${EXP00_LIBRARY_FRONTEND} SHARED src/front.cpp)
set_target_properties(${EXP00_LIBRARY_FRONTEND} PROPERTIES OUTPUT_NAME ${EXP00_LIBRARY_FRONTEND})
set_target_properties(${EXP00_LIBRARY_FRONTEND} PROPERTIES PREFIX "")

# exp00 backend
set(EXP00_LIBRARY_BACKEND exp00_back)
add_library(${EXP00_LIBRARY_BACKEND} SHARED src/back.cpp)
set_target_properties(${EXP00_LIBRARY_BACKEND} PROPERTIES OUTPUT_NAME ${EXP00_LIBRARY_BACKEND})
set_target_properties(${EXP00_LIBRARY_BACKEND} PROPERTIES PREFIX "")

# client
set(EXP_COMMON_FILES
        ${BASE_DIR}/common/include/zmq/zmq_util.cpp
        ${BASE_DIR}/common/include/zmq/socket_cache.cpp
        ${BASE_DIR}/src-gen/common.pb.cc
        ${BASE_DIR}/src-gen/operation.pb.cc
        ${BASE_DIR}/src-gen/kvs.pb.cc
        ${BASE_DIR}/src-gen/anna.pb.cc)

set(EXP_COMMON_LIBRARIES libzmq cppzmq shm-ipc rt fmt yaml-cpp dl ${Protobuf_LIBRARIES})

## register
add_executable(exp00_register
        ${EXP_COMMON_FILES}
        client/register.cpp)
TARGET_LINK_LIBRARIES(exp00_register ${EXP_COMMON_LIBRARIES})

## invoker
add_executable(exp00_invoker
        ${EXP_COMMON_FILES}
        client/invoker.cpp)
TARGET_LINK_LIBRARIES(exp00_invoker ${EXP_COMMON_LIBRARIES})