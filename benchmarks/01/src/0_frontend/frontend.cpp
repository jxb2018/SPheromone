//
// Created by tank on 3/19/23.
//

#include "iostream"
#include "cpp_function.hpp"
#include "utils.h"

using namespace exp01;

extern "C" {
int handle(UserLibraryInterface *library, int arg_size, char **arg_values) {
    int chain_length;
    long will_generate_payload_size;
    // step1: parse input, like  "4 16B"
    bool error = false;
    if (arg_size != 2) {
        std::cerr << "bad input!" << std::endl;
        return -1;
    }
    // parse chain length
    try {
        chain_length = std::stoi(arg_values[0]);
    } catch (std::exception &e) {
        error = true;
    }

    // parse payload size
    will_generate_payload_size = exp01::parse_payload_size(arg_values[1]);
    if (will_generate_payload_size == -1) {
        error = true;
    }

    if (error) {
        std::cerr << "parse request failed!" << std::endl;
        return -1;
    }

    // step2 : generate payload

    // reserve the space for storing chain length
    will_generate_payload_size = will_generate_payload_size < sizeof(int) ? sizeof(int) : will_generate_payload_size;

    auto object = library->create_object("exp01_backend", true, will_generate_payload_size);
    auto out_ptr = reinterpret_cast<int *>(object->get_value());

    // set function chain
    *out_ptr = chain_length;

    // step3: invoke downstream via interface
    auto start_time = exp01::get_timestamp_us();
    library->send_object(object);

    std::cout << "start_time: " << start_time << std::endl;

    return 0;
}
}