//
// Created by mutuxixi on 23-4-4.
//

#include "cpp_function.hpp"
#include "UniqueIdHandler.h"
#include <nlohmann/json.hpp>
#include "utils_for_test.h"

using namespace media_service;

extern "C" {
int handle(UserLibraryInterface *library, int arg_size, char **arg_values) {

    auto start_time = utils::get_timestamp_us();

    using json = nlohmann::json;

    json j = json::parse(arg_values[0]);

    auto req_id  = j["req_id"];

    auto handler = new UniqueIdHandler(library);
    int ret = handler->UploadUniqueId(req_id);

    auto end_time = utils::get_timestamp_us();
    std::cout << "UploadUniqueId1 finished!, token " << end_time - start_time << std::endl;

    return ret;
}
}