//
// Created by tank on 3/22/23.
//
#include <cstring>
#include "cpp_function.hpp"
#include <iostream>
#include "utils.h"
#include <nlohmann/json.hpp>
using json = nlohmann::json;

#define MAX(x, y) x>y?x:y;

extern "C" {
int handle(UserLibraryInterface *library, int arg_size, char **arg_values) {
    //看看获取到的输入是什么格式
    std::cout<<"input is:"<<std::endl;
    for(int i=0;i<arg_size;i++){
        std::cout<<arg_values[i]<<std::endl;
    }
    bool is_valid=true;
    // step1: parse arg_values in json format
    for(int i=1;i<arg_size;i++){//从1开始遍历，因为0是marketdata的数据
        bool valid_last_px=json::parse(arg_values[i]).at("valid").get<bool>();
        is_valid=is_valid&&valid_last_px;
    }
    std::cout<<is_valid<<std::endl;

    return 0;

}
}