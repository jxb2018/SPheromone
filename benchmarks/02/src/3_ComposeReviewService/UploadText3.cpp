//
// Created by mutuxixi on 3/29/23.
//

#include "cpp_function.hpp"
#include "ComposeReviewHandler.h"
#include <nlohmann/json.hpp>

using namespace media_service;

extern "C" {
int handle(UserLibraryInterface *library, int arg_size, char **arg_values) {

    using json = nlohmann::json;

    json j = json::parse(arg_values[0]);

    auto text = new Text();
    text->req_id = j["req_id"];
    strcpy(text->text, j["text"].get<std::string>().c_str());

    auto handler = new ComposeReviewHandler(library);
    int ret = handler->UploadText(*text);

    std::cout << "UploadText3 finished!" << std::endl;

    return ret;
}
}