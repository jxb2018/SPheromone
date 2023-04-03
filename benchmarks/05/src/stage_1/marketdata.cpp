//
// Created by tank on 3/22/23.
//
#include <cstring>
#include "cpp_function.hpp"
#include <iostream>
#include "utils.h"
#include <nlohmann/json.hpp>
#include <fstream>

#define MAX(x, y) x>y?x:y;
using json = nlohmann::json;

extern "C" {
int handle(UserLibraryInterface *library, int arg_size, char **arg_values) {
    std::string tickersForPortfolioTypes[]={"GOOG","AMZN","MSFT"};
    json re;
    for(std::string ticker:tickersForPortfolioTypes){
        usleep(232000);//模拟查询http服务器延迟，yahoo比较慢
    }

    re["GOOG"]="1732.38";
    re["AMZN"]="3185.27";
    re["MSFT"]="221.02";

    std::string re_str=re.dump();



    // step2: generate payload
    auto obj = library->create_object("exp05_marginbalance", true, re_str.size());
    auto val = static_cast<char *>(obj->get_value());
    strncpy(val, re_str.c_str(), re_str.length());
    library->send_object(obj);

    return 0;
}
}