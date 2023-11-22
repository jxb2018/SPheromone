//
// Created by mutuxixi on 23-4-1.
//

#include "cpp_function.hpp"
#include "TextHandler.h"
#include <nlohmann/json.hpp>
#include "utils_for_test.h"

using namespace media_service;

extern "C" {
int handle(UserLibraryInterface *library, int arg_size, char **arg_values) {


    auto start_time = utils::get_timestamp_us();

    using json = nlohmann::json;

    json j = json::parse(arg_values[0]);

    auto text = new Text();
    text->req_id = j["req_id"];
    strcpy(text->text, j["text"].get<std::string>().c_str());

    auto handler = new TextHandler(library);
    int ret = handler->UploadText(*text);


    auto end_time = utils::get_timestamp_us();

    std::cout << "UploadText1 finished!, token " << end_time - start_time << std::endl;

    return ret;
}
}