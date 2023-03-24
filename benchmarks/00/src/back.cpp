//
// Created by tank on 3/23/23.
//

#include <cstring>
#include "cpp_function.hpp"
#include <string>
#include <iostream>
#include "utils.h"

extern "C" {
int handle(UserLibraryInterface *library, int arg_size, char **arg_values) {
    auto end_t = exp00::get_timestamp_us();
    auto serialized = reinterpret_cast<char *>(arg_values[0]);

    int total_chain_len, cur_chain_len;
    long payload_size, send_time;

    int ret = exp00::get_payload(serialized, total_chain_len, cur_chain_len, payload_size, send_time);
    if (ret != 0) {
        std::cerr << "exp00_back: get_payload failed!" << std::endl;
        return -1;
    }

    if (cur_chain_len == 1) {
        std::cout << "chain_length " << total_chain_len <<
                  ", token " << end_t - send_time << " us." << std::endl;
        return 0;
    } else if (cur_chain_len > 1) {
        // step2: generate payload
        auto obj = library->create_object("exp00_back", true, payload_size);
        auto val = static_cast<char *>(obj->get_value());
        exp00::fill(val, payload_size);

        // step3: send to backend
        exp00::set_payload(val, total_chain_len, --cur_chain_len, payload_size, send_time);
        library->send_object(obj);
    } else {
        std::cerr << "bad chain_len : " << cur_chain_len << std::endl;
        return -1;
    }
    return 0;
}
}