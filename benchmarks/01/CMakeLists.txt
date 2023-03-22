include_directories(utils)

set(LIBRARY_OUTPUT_PATH /tmp/shm)

# exp01 front-end
set(LIBRARY_FRONTEND exp01_frontend)
add_library(${LIBRARY_FRONTEND} SHARED src/0_frontend/frontend.cpp)
set_target_properties(${LIBRARY_FRONTEND} PROPERTIES OUTPUT_NAME ${LIBRARY_FRONTEND})
set_target_properties(${LIBRARY_FRONTEND} PROPERTIES PREFIX "")

# exp01 bach-end
set(LIBRARY_BACKEND exp01_backend)
add_library(${LIBRARY_BACKEND} SHARED src/1_backend/backend.cpp)
set_target_properties(${LIBRARY_BACKEND} PROPERTIES OUTPUT_NAME ${LIBRARY_BACKEND})
set_target_properties(${LIBRARY_BACKEND} PROPERTIES PREFIX "")

# client
set(EXP_COMMON_FILES
        ${BASE_DIR}/common/include/zmq/zmq_util.cpp
        ${BASE_DIR}/common/include/zmq/socket_cache.cpp
        ${BASE_DIR}/src-gen/common.pb.cc
        ${BASE_DIR}/src-gen/operation.pb.cc
        ${BASE_DIR}/src-gen/kvs.pb.cc
        ${BASE_DIR}/src-gen/anna.pb.cc)

set(EXP_COMMON_LIBRARIES libzmq cppzmq shm-ipc rt fmt yaml-cpp dl ${Protobuf_LIBRARIES})

add_executable(exp01_client
        ${EXP_COMMON_FILES}
        ${BASE_DIR}/benchmarks/01/client/client.cpp)
TARGET_LINK_LIBRARIES(exp01_client ${EXP_COMMON_LIBRARIES})