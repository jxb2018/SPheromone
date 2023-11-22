//
// Created by mutuxixi on 23-4-1.
//

#include "cpp_function.hpp"
#include "ComposeReviewHandler.h"
#include <nlohmann/json.hpp>
#include "utils_for_test.h"

using namespace media_service;

extern "C" {
int handle(UserLibraryInterface *library, int arg_size, char **arg_values) {

    auto start_time = utils::get_timestamp_us();

    using json = nlohmann::json;

    json j = json::parse(arg_values[0]);

    auto rating  = new Rating();
    rating->req_id = j["req_id"];
    rating->rating = j["rating"];

    auto handler = new ComposeReviewHandler(library);
    int ret = handler->UploadRating(*rating);

    auto end_time = utils::get_timestamp_us();
    std::cout << "UploadRating3 finished!, token " << end_time - start_time << std::endl;

    return ret;
}
}