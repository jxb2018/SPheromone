set(LIBRARY_OUTPUT_PATH /tmp/shm)

# exp01 front-end
set(LIBRARY_FRONTEND exp01_frontend)
add_library(${LIBRARY_FRONTEND} SHARED front_end.cpp)
set_target_properties(${LIBRARY_FRONTEND} PROPERTIES OUTPUT_NAME ${LIBRARY_FRONTEND})
set_target_properties(${LIBRARY_FRONTEND} PROPERTIES PREFIX "")

# exp01 bach-end


# invoker
add_executable(exp01_invoke
        ${BASE_DIR}/common/include/zmq/zmq_util.cpp
        ${BASE_DIR}/common/include/zmq/socket_cache.cpp
        ${BASE_DIR}/src-gen/common.pb.cc
        ${BASE_DIR}/src-gen/operation.pb.cc
        ${BASE_DIR}/src-gen/kvs.pb.cc
        ${BASE_DIR}/src-gen/anna.pb.cc
        ${BASE_DIR}/benchmarks/01/invoke.cpp)

TARGET_LINK_LIBRARIES(exp01_invoke libzmq cppzmq spdlog shm-ipc rt fmt yaml-cpp dl ${Protobuf_LIBRARIES})