set(LIBRARY_OUTPUT_PATH /tmp/shm)

# exp01 front-end
set(LIBRARY_FRONTEND exp00_inc)
add_library(${LIBRARY_FRONTEND} SHARED src/inc.cpp)
set_target_properties(${LIBRARY_FRONTEND} PROPERTIES OUTPUT_NAME ${LIBRARY_FRONTEND})
set_target_properties(${LIBRARY_FRONTEND} PROPERTIES PREFIX "")

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
add_executable(exp00_client
        ${EXP_COMMON_FILES}
        src/client.cpp)
TARGET_LINK_LIBRARIES(exp00_client ${EXP_COMMON_LIBRARIES})