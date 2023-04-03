//
// Created by tank on 3/22/23.
//
#include <cstring>
#include "cpp_function.hpp"
#include <iostream>
#include "utils.h"
#include <nlohmann/json.hpp>

#define MAX(x, y) x>y?x:y;

extern "C" {
void create_and_send_obj(UserLibraryInterface *library,std::string name,int size){
    auto obj = library->create_object(name, true, size);
    auto val = static_cast<char *>(obj->get_value());
    memset(val, 0xff, size);
    val[size - 1] = '\0';
    library->send_object(obj);
}
int handle(UserLibraryInterface *library, int arg_size, char **arg_values) {

    int payload_size=16;
    // step2: generate payload
    create_and_send_obj(library,"exp05_marketdata",16);
    create_and_send_obj(library,"exp05_lastpx",16);
    create_and_send_obj(library,"exp05_side",16);
    create_and_send_obj(library,"exp05_trddate",16);
    create_and_send_obj(library,"exp05_volume",16);


    return 0;

}
}