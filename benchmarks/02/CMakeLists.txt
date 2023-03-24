include_directories(common/)
include_directories(utils/)

set(LIBRARY_OUTPUT_PATH /tmp/shm)

# mongoc-client
find_package(libmongoc-1.0 1.13 REQUIRED)
find_package(Boost 1.54.0 REQUIRED COMPONENTS log log_setup)

include_directories(${MONGOC_INCLUDE_DIRS})

# common library
set(EXP02_COMMON_LIB
        ${MONGOC_LIBRARIES}
        ${Boost_LIBRARIES}
        nlohmann_json mc hiredis fmt)

# one: store review
set(EXP02_STORE_REVIEW_4 exp02_store_review_4)
add_library(${EXP02_STORE_REVIEW_4} SHARED src/4_ReviewStorageService/StoreReview4.cpp)
target_link_libraries(${EXP02_STORE_REVIEW_4} ${EXP02_COMMON_LIB})
set_target_properties(${EXP02_STORE_REVIEW_4} PROPERTIES OUTPUT_NAME ${EXP02_STORE_REVIEW_4})
set_target_properties(${EXP02_STORE_REVIEW_4} PROPERTIES PREFIX "")

# send test review
set(EXP02_SEND_REVIEW_4 exp02_send_review_4)
add_library(${EXP02_SEND_REVIEW_4} SHARED src/4_ReviewStorageService/SendTestReview4.cpp)
target_link_libraries(${EXP02_SEND_REVIEW_4} ${EXP02_COMMON_LIB})
set_target_properties(${EXP02_SEND_REVIEW_4} PROPERTIES OUTPUT_NAME ${EXP02_SEND_REVIEW_4})
set_target_properties(${EXP02_SEND_REVIEW_4} PROPERTIES PREFIX "")

# client
set(EXP_COMMON_FILES
        ${BASE_DIR}/common/include/zmq/zmq_util.cpp
        ${BASE_DIR}/common/include/zmq/socket_cache.cpp
        ${BASE_DIR}/src-gen/common.pb.cc
        ${BASE_DIR}/src-gen/operation.pb.cc
        ${BASE_DIR}/src-gen/kvs.pb.cc
        ${BASE_DIR}/src-gen/anna.pb.cc)

set(EXP_COMMON_LIBRARIES libzmq cppzmq shm-ipc rt fmt yaml-cpp dl ${Protobuf_LIBRARIES})

## store review register
add_executable(exp02_register_store_review_4
        ${EXP_COMMON_FILES}
        ${BASE_DIR}/benchmarks/02/test/register_store_review_4.cpp)
TARGET_LINK_LIBRARIES(exp02_register_store_review_4 ${EXP_COMMON_LIBRARIES})

## store review invoker
add_executable(exp02_invoker_store_review_4
        ${EXP_COMMON_FILES}
        ${BASE_DIR}/benchmarks/02/test/invoker_store_review_4.cpp)
TARGET_LINK_LIBRARIES(exp02_invoker_store_review_4 ${EXP_COMMON_LIBRARIES})