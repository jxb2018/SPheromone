// Created by tank on 3/22/23.
//
#include <cstring>
#include "cpp_function.hpp"
#include <iostream>
#include "utils.h"
#include "nlohmann/json.hpp"
#include <fstream>
#include "ThreadPool.h"

#define MAX(x, y) x>y?x:y;
using json = nlohmann::json;

ThreadPool thread_pool(80);

extern "C" {
std::vector<std::vector<string>> readCSV(string filename) {
    std::ifstream file(filename);
    std::vector<std::vector<string>> data;
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

struct Tuple {
    Tuple(UserLibraryInterface *library,
          std::string func_name,
          std::string re_str) : library(library),
                                func_name(func_name),
                                re_str(re_str) {}

    UserLibraryInterface *library;
    std::string func_name;
    std::string re_str;
};

int create_and_send_obj(void *arg) {

    auto tuple = (Tuple *) arg;
    std::string re_str = tuple->re_str;
    auto library = tuple->library;
    auto name = tuple->func_name;

    re_str += ",";
    re_str += std::to_string(exp05::get_timestamp_us());

    int size = re_str.length();
    auto obj = library->create_object(name, true, size + 1);
    auto val = static_cast<char *>(obj->get_value());
    memset(val, 0, size + 1);
    if (size > 0) {
        strncpy(val, re_str.c_str(), re_str.length());
    } else {
        memset(val, 0xff, size);
        val[size - 1] = '\0';
    }
    library->send_object(obj);

    delete tuple;
    return 0;
}

int handle(UserLibraryInterface *library, int arg_size, char **arg_values) {

    std::string finance_path = getenv("FinraDataPath");
    finance_path += "yfinance_test.csv";
    auto marketdata = readCSV(finance_path);

    json j;
    for (size_t i = 0; i < marketdata.size(); i++) {
        json row;
        for (size_t j = 0; j < marketdata[i].size(); j++) {
            row.push_back(marketdata[i][j]);
        }
        j.push_back(row);
    }
    int fanout_num = std::stoi(arg_values[0]);
    auto req_id = arg_values[1];

    std::string re_str = j.dump();

    std::string fanout_fun_name = "exp06_run_audit";

    std::vector<std::future<int>> results;
    for (int i = 1; i <= fanout_num; i++) {
        std::string func_name = fanout_fun_name + "_" + std::to_string(i);
        auto tuple = new Tuple(library, func_name, req_id);

        results.emplace_back(thread_pool.enqueue(create_and_send_obj, (void *)tuple));
    }

    for(auto && result: results){
        result.get();
    }

//    std::cout << "MarketData REQ_ID : " << req_id << std::endl;

    return 0;
}
}