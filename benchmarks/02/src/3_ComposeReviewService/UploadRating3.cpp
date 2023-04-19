//
// Created by mutuxixi on 23-4-1.
//

#include "cpp_function.hpp"
#include "ComposeReviewHandler.h"
#include <nlohmann/json.hpp>

using namespace media_service;

extern "C" {
int handle(UserLibraryInterface *library, int arg_size, char **arg_values) {

    using json = nlohmann::json;

    json j = json::parse(arg_values[0]);

    auto rating  = new Rating();
    rating->req_id = j["req_id"];
    rating->rating = j["rating"];

    auto handler = new ComposeReviewHandler(library);
    int ret = handler->UploadRating(*rating);

//    std::cout << "UploadRating3 finished!" << std::endl;

    return ret;
}
}