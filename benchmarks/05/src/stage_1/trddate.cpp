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
bool is_date(std::string str){
    int month[12]={31,28,31,30,31,30,31,31,30,31,30,31};
    int y=std::atoi(str.substr(0,2).c_str());
    int m=std::atoi(str.substr(2,2).c_str());
    int d=std::atoi(str.substr(4,2).c_str());

    if(y<=23){
        y+=2000;
    }else{
        y+=1900;
    }
    if((y%4==0&&y%100!=0)||(y%400==0)){//如果是闰年
        month[1] = 29;
        if(0<d&& d<=month[m-1] && m>0 && m<=12)
            return true;
        else
            return false;
    }
    else{
        if(0<d&& d<=month[m-1] && 0<m && 12>=m)
            return true;
        else
            return false;
    }

    return false;
}
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
        auto str=j["1234"][i].at("TradeDate").get<std::string>();
        if (str.length()!=6||!is_date(str)){
            valid= false;
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