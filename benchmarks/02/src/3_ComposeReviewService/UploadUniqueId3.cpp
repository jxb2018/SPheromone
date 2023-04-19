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

    auto unique_id  = new UniqueId();
    unique_id->req_id = j["req_id"];
    unique_id->review_id = j["review_id"];

    auto handler = new ComposeReviewHandler(library);
    int ret = handler->UploadUniqueId(*unique_id);

//    std::cout << "UploadUniqueId3 finished!" << std::endl;

    return ret;
}
}