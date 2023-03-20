//
// Created by tank on 3/19/23.
//
#include "iostream"
#include "cpp_function.hpp"
#include "utils.h"

using namespace exp01;

extern "C" {
int handle(UserLibraryInterface *library, int arg_size, char **arg_values) {

    // step1: parse request or get from store
//    auto chain_length = std::stoi(arg_values[0]);

    auto object = library->get_object("b_exp01_backend", "k_exp01_frontend", true);
    auto data_ptr = reinterpret_cast<char *>(object->get_value());

    auto chain_length = std::stoi(data_ptr);
    if (chain_length < 1 || chain_length > 128) {
        std::cerr << "bad chain length!" << std::endl;
        return -1;
    }
    if (chain_length == 1) {
        auto end_time = exp01::get_timestamp_us();
        std::cout << "end_time: " << end_time << std::endl;
        return 0;
    }

    // step2 : update chain length
    strcpy(data_ptr, std::to_string(--chain_length).c_str());

    // step 3: invoke downstream via interface
    library->send_object(object);

    return 0;
}
}