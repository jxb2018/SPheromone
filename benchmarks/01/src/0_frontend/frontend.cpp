//
// Created by tank on 3/19/23.
//

#include "iostream"
#include "cpp_function.hpp"
#include "utils.h"
#include <vector>

using namespace exp01;

#define MAX(x, y) x>y?x:y;

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
    will_generate_payload_size = MAX(64, will_generate_payload_size);

    // step3: via interface
    EpheObject *obj;
    if(will_generate_payload_size < 1024){
        obj = library->create_object(will_generate_payload_size);
    }else{
        obj = library->create_object("exp01_backend", true, will_generate_payload_size);
    }
    auto val = (char *)(obj->get_value());
    memset(val, 'a', will_generate_payload_size);
    val[will_generate_payload_size - 1] = '\0';

    std::cout << "send payload, size = " << will_generate_payload_size << ", start_time = " << exp01::get_timestamp_us() << std::endl;
    library->send_object(obj);

    return 0;
}
}