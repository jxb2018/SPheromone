SET(SCHEDULER_SOURCE
        ${BASE_DIR}/common/include/zmq/zmq_util.cpp
        ${BASE_DIR}/common/include/zmq/socket_cache.cpp
        ${BASE_DIR}/src/scheduler/scheduler.cpp
        ${BASE_DIR}/src/scheduler/object_handlers.cpp
        ${BASE_DIR}/src-gen/common.pb.cc
        ${BASE_DIR}/src-gen/operation.pb.cc
        ${BASE_DIR}/src-gen/kvs.pb.cc
        ${BASE_DIR}/src-gen/anna.pb.cc)

ADD_EXECUTABLE(scheduler ${SCHEDULER_SOURCE})
TARGET_LINK_LIBRARIES(scheduler libzmq cppzmq shm-ipc rt fmt yaml-cpp dl ${Protobuf_LIBRARIES})