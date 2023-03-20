//
// Created by tank on 3/19/23.
//
#include "iostream"
#include "cpp_function.hpp"
#include "utils.h"

using namespace exp01;

extern "C" {
int handle(UserLibraryInterface *library, int arg_size, char **arg_values) {
    // step1: parse request
    if (strlen(arg_values[0]) < sizeof(int)) {
        std::cerr << "bad input length!" << std::endl;
        return -1;
    }
    auto chain_length = reinterpret_cast<int *>(arg_values[0]);
    if (*chain_length < 1 || *chain_length > 128) {
        std::cerr << "bad chain length!" << std::endl;
        return -1;
    }

    if (*chain_length == 1) {
        auto end_time = exp01::get_timestamp_us();
        std::cout << "end_time: " << end_time << std::endl;
        return 0;
    }

    // step 2: prepare payload

    // step 3: invoke downstream via interface
    return 0;
}
}