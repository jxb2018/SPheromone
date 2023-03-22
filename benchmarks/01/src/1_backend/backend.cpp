//
// Created by tank on 3/19/23.
//
#include "iostream"
#include "cpp_function.hpp"
#include "utils.h"

using namespace exp01;

extern "C" {
int handle(UserLibraryInterface *library, int arg_size, char **arg_values) {

    std::cout << "recv payload, size = " << strlen(arg_values[0]) << ", end_time = " << exp01::get_timestamp_us() << std::endl;
    return 0;
}
}