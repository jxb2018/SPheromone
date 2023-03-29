//
// Created by mutuxixi on 23-3-24.
//

#include "cpp_function.hpp"
#include "utils_for_test.h"
#include <nlohmann/json.hpp>
#include "ReviewStorageHandler.h"
#include <cstring>

using namespace media_service;

extern "C" {
int handle(UserLibraryInterface *library, int arg_size, char **arg_values) {

    using json = nlohmann::json;

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

    json j;
    j["review_id"]=review->review_id;
    j["user_id"]=review->user_id;
    j["req_id"]=review->req_id;
    j["text"]=review->text;
    j["movie_id"]=review->movie_id;
    j["rating"]=review->rating;
    j["timestamp"]=review->timestamp;

    string str=j.dump();
    std::cout<<str<<std::endl;
    auto obj = library->create_object("exp02_store_review_4", true, 2048);

    auto val = (char *) (obj->get_value());
    memset(val, 0, 2048);
    strcpy(val, str.c_str());

    library->send_object(obj);

    std::cout<<"send finished!"<<std::endl;

    return 0;
}
}