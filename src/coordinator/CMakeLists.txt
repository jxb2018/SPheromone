SET(COORDINATOR_SOURCE
        ${BASE_DIR}/common/include/zmq/zmq_util.cpp
        ${BASE_DIR}/common/include/zmq/socket_cache.cpp
        ${BASE_DIR}/src/coordinator/coordinator.cpp
        ${BASE_DIR}/src/coordinator/notify_handler.cpp
        ${BASE_DIR}/src/coordinator/query_handler.cpp
        ${BASE_DIR}/src/coordinator/bucket_op_handler.cpp
        ${BASE_DIR}/src/coordinator/trigger_op_handler.cpp
        ${BASE_DIR}/src/coordinator/func_call_handler.cpp
        ${BASE_DIR}/src/coordinator/app_register_handler.cpp
        ${BASE_DIR}/src-gen/common.pb.cc
        ${BASE_DIR}/src-gen/operation.pb.cc
        ${BASE_DIR}/src-gen/kvs.pb.cc
        ${BASE_DIR}/src-gen/anna.pb.cc)

ADD_EXECUTABLE(coordinator ${COORDINATOR_SOURCE})
TARGET_LINK_LIBRARIES(coordinator libzmq cppzmq shm-ipc rt fmt yaml-cpp dl ${Protobuf_LIBRARIES})