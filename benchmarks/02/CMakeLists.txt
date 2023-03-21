include_directories(common/)
include_directories(utils/)

# mongoc-client
find_package(libmongoc-1.0 1.13 REQUIRED)
find_package(Boost 1.54.0 REQUIRED COMPONENTS log log_setup)

include_directories(${MONGOC_INCLUDE_DIRS})

# common library
set(EXP02_COMMON_LIB
        ${MONGOC_LIBRARIES}
        ${Boost_LIBRARIES}
        nlohmann_json mc hiredis)

# one: store review
set(EXP02_STORE_REVIEW_4 exp02_store_review_4)
add_library(${EXP02_STORE_REVIEW_4} SHARED src/4_ReviewStorageService/StoreReview4.cpp)
target_link_libraries(${EXP02_STORE_REVIEW_4} ${EXP02_COMMON_LIB})
set_target_properties(${EXP02_STORE_REVIEW_4} PROPERTIES OUTPUT_NAME ${EXP02_STORE_REVIEW_4})
set_target_properties(${EXP02_STORE_REVIEW_4} PROPERTIES PREFIX "")