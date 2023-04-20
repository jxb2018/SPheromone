set(LIBRARY_OUTPUT_PATH /tmp/shm)

include_directories(utils/)

set(EXP06_LIBRARY_MARKETDATA exp06_marketdata)
add_library(${EXP06_LIBRARY_MARKETDATA} SHARED src/stage_0/marketdata.cpp)
target_link_libraries(${EXP06_LIBRARY_MARKETDATA} pthread)
set_target_properties(${EXP06_LIBRARY_MARKETDATA} PROPERTIES OUTPUT_NAME ${EXP06_LIBRARY_MARKETDATA})
set_target_properties(${EXP06_LIBRARY_MARKETDATA} PROPERTIES PREFIX "")

set(EXP06_LIBRARY_RUNAUDIT exp06_run_audit)
add_library(${EXP06_LIBRARY_RUNAUDIT} SHARED src/stage_1/run_audit.cpp)
set_target_properties(${EXP06_LIBRARY_RUNAUDIT} PROPERTIES OUTPUT_NAME ${EXP06_LIBRARY_RUNAUDIT})
set_target_properties(${EXP06_LIBRARY_RUNAUDIT} PROPERTIES PREFIX "")

set(EXP06_LIBRARY_MARGINBALANCE exp06_marginbalance)
add_library(${EXP06_LIBRARY_MARGINBALANCE} SHARED src/stage_2/marginBalance.cpp)
target_link_libraries(${EXP06_LIBRARY_MARGINBALANCE} fmt)
set_target_properties(${EXP06_LIBRARY_MARGINBALANCE} PROPERTIES OUTPUT_NAME ${EXP06_LIBRARY_MARGINBALANCE})
set_target_properties(${EXP06_LIBRARY_MARGINBALANCE} PROPERTIES PREFIX "")

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
add_executable(exp06_register
        ${EXP_COMMON_FILES}
        client/register.cpp)
TARGET_LINK_LIBRARIES(exp06_register ${EXP_COMMON_LIBRARIES})

## invoker
add_executable(exp06_invoker
        ${EXP_COMMON_FILES}
        client/invoker.cpp)
TARGET_LINK_LIBRARIES(exp06_invoker ${EXP_COMMON_LIBRARIES})