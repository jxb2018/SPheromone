//
// Created by mutuxixi on 23-3-24.
//

#include "cpp_function.hpp"
#include "utils_for_test.h"
#include "ReviewStorageHandler.h"

using namespace media_service;

extern "C" {
int handle(UserLibraryInterface *library, int arg_size, char **arg_values) {

    auto obj = library->create_object("exp02_store_function_4", true, sizeof(Review)+5);

    // make Review
    srand(time(nullptr));
    char *buf = (char *) malloc(1024);

    auto review = reinterpret_cast<Review *>(buf);
    review->review_id = rand();
    review->user_id = rand();
    review->req_id = rand();
    strcpy(review->text, utils::RandomString(64).c_str());
    strcpy(review->movie_id, utils::RandomString(64).c_str());
    review->rating = rand();
    review->timestamp = utils::get_timestamp_us();

    auto val = (char *) (obj->get_value());
    memcpy(val, buf, sizeof(Review));
    val[sizeof(Review)]='\0';

    library->send_object(obj);

    return 0;
}
}