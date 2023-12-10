//
// Created by tank on 3/22/23.
//
#include <cstring>
#include "cpp_function.hpp"
#include <iostream>
#include "utils.h"
#include <nlohmann/json.hpp>
#include "utils_for_test.h"

using json = nlohmann::json;

#define MAX(x, y) x>y?x:y;

auto gateway_addr = utils::GetEnvVariable("LUMINE_GATEWAY_ADDR", "");
auto gateway_port = utils::GetEnvVariable("LUMINE_GATEWAY_PORT", "");

//auto request = utils::Socket(gateway_addr.c_str(), std::stoi(gateway_port));

extern "C" {
bool checkMarginBalance(char *marketData) {
    return true;
}
int handle(UserLibraryInterface *library, int arg_size, char **arg_values) {
    int size = 0;

    for (int i = 0; i < arg_size; i++) {
        size += strlen(arg_values[i]);
    }
//    std::cout << "marginBalance: input size is:" << size << std::endl;

    json j = json::parse(arg_values[0]);

    bool is_valid = true;
    std::string req_id;
    for (int i = 0; i < arg_size; i++) {
        bool valid_last_px = j.at("valid").get<bool>();
        is_valid = is_valid && valid_last_px;
        req_id = j.at("req_id").get<std::string>();
    }
    auto marketData = arg_values[0];
    bool marginSatisfied = checkMarginBalance(marketData);

//    if (!gateway_addr.empty() && !gateway_port.empty()) {
//        if (request.conn() < 0) {
//            perror("Failed to connect");
//            return -1;
//        }
//        request.issue_http_request("POST", "/function/exp09End", req_id.c_str());
//    }

    auto end_t = exp05::get_timestamp_us();

    string id, start_t_str;

    size_t pos_delim = req_id.find(',');
    if(pos_delim != -1){
        id = req_id.substr(0, pos_delim);
        start_t_str = req_id.substr(pos_delim + 1, req_id.size() - id.size() - 1);
    }

    std::cout << "req_id = " << id <<
              ", token " << end_t - std::stol(start_t_str) << " us." << std::endl;

//    if (is_valid && marginSatisfied) {
//        std::cout << "MarginBalance, succeed, " << req_id << std::endl;
//    } else {
//        std::cout << "MarginBalance, failed, " << req_id << std::endl;
//    }

    return 0;

}
}