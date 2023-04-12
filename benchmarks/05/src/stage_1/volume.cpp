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
    char * data_path;
    if(!(data_path= getenv("FinraDataPath"))){
        perror("please set env FinraDataPath");
    }
    json j;
    std::ifstream jfile(std::string(data_path)+"portfolios.json");
    jfile>>j;

    int tilength = j["1234"].size();
    bool valid=true;
    //lastpx是浮点数（为了保证精度，使用字符串存储），查看小数点左右的位数，不符合要求返回false
    for (int i = 0; i < tilength; i++) {
        auto str=j["1234"][i].at("LastQty").get<std::string>();
        if((str.find(".")!=std::string::npos)||str.length()>8){
            valid = false;
            break;
        }
    }

    json re;
    re["valid"]=valid;
    std::string re_str=re.dump();


    // step2: generate payload
    auto obj = library->create_object("exp05_marginbalance", true, re_str.size());
    auto val = static_cast<char *>(obj->get_value());
    strncpy(val, re_str.c_str(), re_str.length());
    library->send_object(obj);

    return 0;
}
}