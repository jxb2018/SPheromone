//
// Created by mutuxixi on 23-4-1.
//

#include "cpp_function.hpp"
#include "TextHandler.h"
#include <nlohmann/json.hpp>

using namespace media_service;

extern "C" {
int handle(UserLibraryInterface *library, int arg_size, char **arg_values) {

    using json = nlohmann::json;

    json j = json::parse(arg_values[0]);

    auto text = new Text();
    text->req_id = j["req_id"];
    strcpy(text->text, j["text"].get<std::string>().c_str());

    auto handler = new TextHandler(library);
    int ret = handler->UploadText(*text);

//    std::cout << "UploadText1 finished!" << std::endl;

    return ret;
}
}