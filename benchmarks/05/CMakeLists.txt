set(LIBRARY_OUTPUT_PATH /tmp/shm_dzl)

include_directories(utils/)

set(EXP05_LIBRARY_FRONTEND exp05_front)
add_library(${EXP05_LIBRARY_FRONTEND} SHARED src/stage_0/front.cpp)
set_target_properties(${EXP05_LIBRARY_FRONTEND} PROPERTIES OUTPUT_NAME ${EXP05_LIBRARY_FRONTEND})
set_target_properties(${EXP05_LIBRARY_FRONTEND} PROPERTIES PREFIX "")

set(EXP05_LIBRARY_LASTPX exp05_lastpx)
add_library(${EXP05_LIBRARY_LASTPX} SHARED src/stage_1/lastpx.cpp)
set_target_properties(${EXP05_LIBRARY_LASTPX} PROPERTIES OUTPUT_NAME ${EXP05_LIBRARY_LASTPX})
set_target_properties(${EXP05_LIBRARY_LASTPX} PROPERTIES PREFIX "")

set(EXP05_LIBRARY_MARKETDATA exp05_marketdata)
add_library(${EXP05_LIBRARY_MARKETDATA} SHARED src/stage_1/marketdata.cpp)
set_target_properties(${EXP05_LIBRARY_MARKETDATA} PROPERTIES OUTPUT_NAME ${EXP05_LIBRARY_MARKETDATA})
set_target_properties(${EXP05_LIBRARY_MARKETDATA} PROPERTIES PREFIX "")

set(EXP05_LIBRARY_SIDE exp05_side)
add_library(${EXP05_LIBRARY_SIDE} SHARED src/stage_1/side.cpp)
set_target_properties(${EXP05_LIBRARY_SIDE} PROPERTIES OUTPUT_NAME ${EXP05_LIBRARY_SIDE})
set_target_properties(${EXP05_LIBRARY_SIDE} PROPERTIES PREFIX "")

set(EXP05_LIBRARY_TRDDATE exp05_trddate)
add_library(${EXP05_LIBRARY_TRDDATE} SHARED src/stage_1/trddate.cpp)
set_target_properties(${EXP05_LIBRARY_TRDDATE} PROPERTIES OUTPUT_NAME ${EXP05_LIBRARY_TRDDATE})
set_target_properties(${EXP05_LIBRARY_TRDDATE} PROPERTIES PREFIX "")

set(EXP05_LIBRARY_VOLUME exp05_volume)
add_library(${EXP05_LIBRARY_VOLUME} SHARED src/stage_1/volume.cpp)
set_target_properties(${EXP05_LIBRARY_VOLUME} PROPERTIES OUTPUT_NAME ${EXP05_LIBRARY_VOLUME})
set_target_properties(${EXP05_LIBRARY_VOLUME} PROPERTIES PREFIX "")

set(EXP05_LIBRARY_MARGINBALANCE exp05_marginbalance)
add_library(${EXP05_LIBRARY_MARGINBALANCE} SHARED src/stage_2/marginBalance.cpp)
set_target_properties(${EXP05_LIBRARY_MARGINBALANCE} PROPERTIES OUTPUT_NAME ${EXP05_LIBRARY_MARGINBALANCE})
set_target_properties(${EXP05_LIBRARY_MARGINBALANCE} PROPERTIES PREFIX "")

# client
set(EXP_COMMON_FILES
        ${BASE_DIR}/common/include/zmq/zmq_util.cpp
        ${BASE_DIR}/common/include/zmq/socket_cache.cpp
        ${BASE_DIR}/src-gen/common.pb.cc
        ${BASE_DIR}/src-gen/operation.pb.cc
        ${BASE_DIR}/src-gen/kvs.pb.cc
        ${BASE_DIR}/src-gen/anna.pb.cc)

set(EXP_COMMON_LIBRARIES nlohmann_json libzmq cppzmq shm-ipc rt fmt yaml-cpp dl ${Protobuf_LIBRARIES})

## register
add_executable(exp05_register
        ${EXP_COMMON_FILES}
        client/register.cpp)
TARGET_LINK_LIBRARIES(exp05_register ${EXP_COMMON_LIBRARIES})

## invoker
add_executable(exp05_invoker
        ${EXP_COMMON_FILES}
        client/invoker.cpp)
TARGET_LINK_LIBRARIES(exp05_invoker ${EXP_COMMON_LIBRARIES})