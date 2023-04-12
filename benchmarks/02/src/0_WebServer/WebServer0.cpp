//
// Created by mutuxixi on 23-4-1.
//

#include "cpp_function.hpp"
#include <nlohmann/json.hpp>
#include <iostream>
#include "utils_for_test.h"

extern "C" {
int handle(UserLibraryInterface *library, int arg_size, char **arg_values) {

    using json = nlohmann::json;

    json j;
    j["req_id"] = 1;
    j["rating"] = 1;
    j["title"] = utils::RandomString(64);
    j["text"] = utils::RandomString(64);
    j["movie_id"] = utils::RandomString(64);
//    j["title"] = "aaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaa";
//    j["text"] = "bbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbb";
//    j["movie_id"] = "cccccccccccccccccccccccccccccccccccccccccccccccccccccccccccccccc";

    string str = j.dump();
    // text register movieid
    auto obj = library->create_object("exp02_register_movieid_1", true, 1024);
    auto val = (char *) (obj->get_value());
    memset(val, 0, 1024);
    strcpy(val, str.c_str());
    library->send_object(obj);

    // text text service
    auto obj1 = library->create_object("exp02_upload_text_1", true, 1024);
    auto val1 = (char *) (obj1->get_value());
    memset(val1, 0, 1024);
    strcpy(val1, str.c_str());
    library->send_object(obj1);

    // text movieid service
    auto obj2 = library->create_object("exp02_upload_movieid_1", true, 1024);
    auto val2 = (char *) (obj2->get_value());
    memset(val2, 0, 1024);
    strcpy(val2, str.c_str());
    library->send_object(obj2);

    // text uniqueid service
    auto obj3 = library->create_object("exp02_upload_uniqueid_1", true, 1024);
    auto val3 = (char *) (obj3->get_value());
    memset(val3, 0, 1024);
    strcpy(val3, str.c_str());
    library->send_object(obj3);

    std::cout << "WebServer0 finished!" << std::endl;

    return 0;
}
}