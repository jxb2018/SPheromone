//
// Created by tank on 3/22/23.
//
#include <cstring>
#include "cpp_function.hpp"
#include <iostream>
#include "utils.h"

#define MAX(x, y) x>y?x:y;

extern "C" {
int handle(UserLibraryInterface *library, int arg_size, char **arg_values) {
    int chain_length;
    long payload_size;

    // step1: parse input
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
    payload_size = exp00::parse_payload_size(arg_values[1]);
    if (payload_size == -1) {
        error = true;
    }

    payload_size = MAX(payload_size, 64);

    if (error) {
        std::cerr << "parse request failed!" << std::endl;
        return -1;
    }

    // step2: generate payload
    auto obj = library->create_object("exp00_back", true, payload_size);
    auto val = static_cast<char *>(obj->get_value());
    exp00::fill(val, payload_size);

    // step3: send to backend
    long start_time = exp00::get_timestamp_us();
    exp00::set_payload(val, chain_length, chain_length, payload_size, start_time);
    library->send_object(obj);
    return 0;
}
}