//
// Created by mutuxixi on 23-4-4.
//

#include "cpp_function.hpp"
#include "UniqueIdHandler.h"
#include <nlohmann/json.hpp>

using namespace media_service;

extern "C" {
int handle(UserLibraryInterface *library, int arg_size, char **arg_values) {

    using json = nlohmann::json;

    json j = json::parse(arg_values[0]);

    auto req_id  = j["req_id"];

    auto handler = new UniqueIdHandler(library);
    int ret = handler->UploadUniqueId(req_id);

//    std::cout << "UploadUniqueId1 finished!" << std::endl;

    return ret;
}
}