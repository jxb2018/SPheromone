SET(MANAGEMENT_SOURCE
        ${BASE_DIR}/src/manager/manager.cpp
        ${BASE_DIR}/common/include/zmq/zmq_util.cpp
        ${BASE_DIR}/src-gen/common.pb.cc
        ${BASE_DIR}/src-gen/operation.pb.cc)

ADD_EXECUTABLE(manager ${MANAGEMENT_SOURCE})
TARGET_LINK_LIBRARIES(manager libzmq cppzmq spdlog shm-ipc rt fmt yaml-cpp dl ${PROTOBUF_LIBRARIES})