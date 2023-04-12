include_directories(common/)
include_directories(utils/)

set(LIBRARY_OUTPUT_PATH /tmp/shm)

# client
set(EXP_COMMON_FILES
        ${BASE_DIR}/common/include/zmq/zmq_util.cpp
        ${BASE_DIR}/common/include/zmq/socket_cache.cpp
        ${BASE_DIR}/src-gen/common.pb.cc
        ${BASE_DIR}/src-gen/operation.pb.cc
        ${BASE_DIR}/src-gen/kvs.pb.cc
        ${BASE_DIR}/src-gen/anna.pb.cc)

set(EXP_COMMON_LIBRARIES libzmq cppzmq shm-ipc rt fmt yaml-cpp dl ${Protobuf_LIBRARIES})

## test1 register
add_executable(exp02_register_test1
        ${EXP_COMMON_FILES}
        ${BASE_DIR}/benchmarks/02/test/register_test1.cpp)
TARGET_LINK_LIBRARIES(exp02_register_test1 ${EXP_COMMON_LIBRARIES})

## test1 invoker
add_executable(exp02_invoker_test1
        ${EXP_COMMON_FILES}
        ${BASE_DIR}/benchmarks/02/test/invoker_test1.cpp)
TARGET_LINK_LIBRARIES(exp02_invoker_test1 ${EXP_COMMON_LIBRARIES})

# mongoc-client
find_package(libmongoc-1.0 1.13 REQUIRED)
find_package(Boost 1.54.0 REQUIRED COMPONENTS log log_setup)

include_directories(${MONGOC_INCLUDE_DIRS})

# common library
set(EXP02_COMMON_LIB
        ${MONGOC_LIBRARIES}
        ${Boost_LIBRARIES}
        nlohmann_json mc hiredis fmt)

# 4 store review
set(EXP02_STORE_REVIEW_4 exp02_store_review_4)
add_library(${EXP02_STORE_REVIEW_4} SHARED src/4_ReviewStorageService/StoreReview4.cpp)
target_link_libraries(${EXP02_STORE_REVIEW_4} ${EXP02_COMMON_LIB})
set_target_properties(${EXP02_STORE_REVIEW_4} PROPERTIES OUTPUT_NAME ${EXP02_STORE_REVIEW_4})
set_target_properties(${EXP02_STORE_REVIEW_4} PROPERTIES PREFIX "")

# 3 Upload Text
set(EXP02_UPLOAD_TEXT_3 exp02_upload_text_3)
add_library(${EXP02_UPLOAD_TEXT_3} SHARED src/3_ComposeReviewService/UploadText3.cpp)
target_link_libraries(${EXP02_UPLOAD_TEXT_3} ${EXP02_COMMON_LIB})
set_target_properties(${EXP02_UPLOAD_TEXT_3} PROPERTIES OUTPUT_NAME ${EXP02_UPLOAD_TEXT_3})
set_target_properties(${EXP02_UPLOAD_TEXT_3} PROPERTIES PREFIX "")

# 3 Upload UniqueId
set(EXP02_UPLOAD_UNIQUEID_3 exp02_upload_uniqueid_3)
add_library(${EXP02_UPLOAD_UNIQUEID_3} SHARED src/3_ComposeReviewService/UploadUniqueId3.cpp)
target_link_libraries(${EXP02_UPLOAD_UNIQUEID_3} ${EXP02_COMMON_LIB})
set_target_properties(${EXP02_UPLOAD_UNIQUEID_3} PROPERTIES OUTPUT_NAME ${EXP02_UPLOAD_UNIQUEID_3})
set_target_properties(${EXP02_UPLOAD_UNIQUEID_3} PROPERTIES PREFIX "")

# 3 Upload Rating
set(EXP02_UPLOAD_RATING_3 exp02_upload_rating_3)
add_library(${EXP02_UPLOAD_RATING_3} SHARED src/3_ComposeReviewService/UploadRating3.cpp)
target_link_libraries(${EXP02_UPLOAD_RATING_3} ${EXP02_COMMON_LIB})
set_target_properties(${EXP02_UPLOAD_RATING_3} PROPERTIES OUTPUT_NAME ${EXP02_UPLOAD_RATING_3})
set_target_properties(${EXP02_UPLOAD_RATING_3} PROPERTIES PREFIX "")

# 3 Upload MovieId
set(EXP02_UPLOAD_MOVIEID_3 exp02_upload_movieid_3)
add_library(${EXP02_UPLOAD_MOVIEID_3} SHARED src/3_ComposeReviewService/UploadMovieId3.cpp)
target_link_libraries(${EXP02_UPLOAD_MOVIEID_3} ${EXP02_COMMON_LIB})
set_target_properties(${EXP02_UPLOAD_MOVIEID_3} PROPERTIES OUTPUT_NAME ${EXP02_UPLOAD_MOVIEID_3})
set_target_properties(${EXP02_UPLOAD_MOVIEID_3} PROPERTIES PREFIX "")

# 2 Upload Rating
set(EXP02_UPLOAD_RATING_2 exp02_upload_rating_2)
add_library(${EXP02_UPLOAD_RATING_2} SHARED src/2_RatingService/UploadRating2.cpp)
target_link_libraries(${EXP02_UPLOAD_RATING_2} ${EXP02_COMMON_LIB})
set_target_properties(${EXP02_UPLOAD_RATING_2} PROPERTIES OUTPUT_NAME ${EXP02_UPLOAD_RATING_2})
set_target_properties(${EXP02_UPLOAD_RATING_2} PROPERTIES PREFIX "")

# 1 Upload UniqueId
set(EXP02_UPLOAD_UNIQUEID_1 exp02_upload_uniqueid_1)
add_library(${EXP02_UPLOAD_UNIQUEID_1} SHARED src/1_UniqueIdService/UploadUniqueId1.cpp)
target_link_libraries(${EXP02_UPLOAD_UNIQUEID_1} ${EXP02_COMMON_LIB})
set_target_properties(${EXP02_UPLOAD_UNIQUEID_1} PROPERTIES OUTPUT_NAME ${EXP02_UPLOAD_UNIQUEID_1})
set_target_properties(${EXP02_UPLOAD_UNIQUEID_1} PROPERTIES PREFIX "")

# 1 Upload Text
set(EXP02_UPLOAD_TEXT_1 exp02_upload_text_1)
add_library(${EXP02_UPLOAD_TEXT_1} SHARED src/1_TextService/UploadText1.cpp)
target_link_libraries(${EXP02_UPLOAD_TEXT_1} ${EXP02_COMMON_LIB})
set_target_properties(${EXP02_UPLOAD_TEXT_1} PROPERTIES OUTPUT_NAME ${EXP02_UPLOAD_TEXT_1})
set_target_properties(${EXP02_UPLOAD_TEXT_1} PROPERTIES PREFIX "")

# 1 Upload MovieId
set(EXP02_UPLOAD_MOVIEID_1 exp02_upload_movieid_1)
add_library(${EXP02_UPLOAD_MOVIEID_1} SHARED src/1_MovieIdService/UploadMovieId1.cpp)
target_link_libraries(${EXP02_UPLOAD_MOVIEID_1} ${EXP02_COMMON_LIB})
set_target_properties(${EXP02_UPLOAD_MOVIEID_1} PROPERTIES OUTPUT_NAME ${EXP02_UPLOAD_MOVIEID_1})
set_target_properties(${EXP02_UPLOAD_MOVIEID_1} PROPERTIES PREFIX "")

# 1 Register MovieId
set(EXP02_REGISTER_MOVIEID_1 exp02_register_movieid_1)
add_library(${EXP02_REGISTER_MOVIEID_1} SHARED src/1_MovieIdService/RegisterMovieId.cpp)
target_link_libraries(${EXP02_REGISTER_MOVIEID_1} ${EXP02_COMMON_LIB})
set_target_properties(${EXP02_REGISTER_MOVIEID_1} PROPERTIES OUTPUT_NAME ${EXP02_REGISTER_MOVIEID_1})
set_target_properties(${EXP02_REGISTER_MOVIEID_1} PROPERTIES PREFIX "")

# 0 Web Server
set(EXP02_WEB_SERVER_0 exp02_web_server_0)
add_library(${EXP02_WEB_SERVER_0} SHARED src/0_WebServer/WebServer0.cpp)
target_link_libraries(${EXP02_WEB_SERVER_0} ${EXP02_COMMON_LIB})
set_target_properties(${EXP02_WEB_SERVER_0} PROPERTIES OUTPUT_NAME ${EXP02_WEB_SERVER_0})
set_target_properties(${EXP02_WEB_SERVER_0} PROPERTIES PREFIX "")