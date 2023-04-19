// Created by tank on 3/22/23.
//
#include <cstring>
#include "cpp_function.hpp"
#include <iostream>
#include "utils.h"
#include "nlohmann/json.hpp"
#include <fstream>

#define MAX(x, y) x>y?x:y;
using json = nlohmann::json;

extern "C" {
std::vector<std::vector<string>> readCSV(string filename) {
    std::ifstream file(filename);
    std::vector< std::vector<string>> data;
    string line;

    while (getline(file, line)) {
        std::vector<string> row;
        std::stringstream lineStream(line);
        string cell;

        while (getline(lineStream, cell, ',')) {
            row.push_back(cell);
        }

        data.push_back(row);
    }

    return data;
}
void create_and_send_obj(UserLibraryInterface *library,const std::string & name,const std::string & re_str){
    int size=re_str.length();
    auto obj = library->create_object(name, true, size);
    auto val = static_cast<char *>(obj->get_value());
    if(re_str.length()>0){
        strncpy(val, re_str.c_str(), re_str.length());
    }else{
        memset(val, 0xff, size);
        val[size - 1] = '\0';
    }
    library->send_object(obj);
}

int handle(UserLibraryInterface *library, int arg_size, char **arg_values) {
    auto marketdata=readCSV("../data/yfinance_test.csv");
    json j;
    for (size_t i = 0; i < marketdata.size(); i++) {
        json row;
        for (size_t j = 0; j < marketdata[i].size(); j++) {
            row.push_back(marketdata[i][j]);
        }
        j.push_back(row);
    }
    int fanout_num=std::stoi(arg_values[0]);

    std::string re_str=j.dump();

    std::string fanout_fun_name="exp06_run_audit";
    for (int i = 1; i <= fanout_num; i++) {
        std::string func_name = fanout_fun_name + "_" + std::to_string(i);
        create_and_send_obj(library,func_name,re_str);
    }

    return 0;
}
}