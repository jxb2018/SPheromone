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
    bool checkMarginBalance(char* marketData){
        return true;
    }
int handle(UserLibraryInterface *library, int arg_size, char **arg_values) {
    int size=0;
    for(int i=0;i<arg_size;i++){
        size+=strlen(arg_values[i]);
    }
    std::cout<<"input size is:"<<size<<std::endl;

    json j = json::parse(arg_values[0]);

    bool is_valid=true;
    for(int i=0;i<arg_size;i++){
        bool valid_last_px=j.at("valid").get<bool>();
        is_valid=is_valid&&valid_last_px;
    }
    auto marketData=arg_values[0];
    bool marginSatisfied = checkMarginBalance(marketData);
    std::cout<<(is_valid&&marginSatisfied)<<std::endl;

    return 0;

}
}