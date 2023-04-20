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

//void create_and_send_obj(UserLibraryInterface *library, const std::string &name, const std::string &re_str) {
//    int size = re_str.length();
//    auto obj = library->create_object(name, true, size);
//    auto val = static_cast<char *>(obj->get_value());
//    if (re_str.length() > 0) {
//        strncpy(val, re_str.c_str(), re_str.length());
//    } else {
//        memset(val, 0xff, size);
//        val[size - 1] = '\0';
//    }
//    library->send_object(obj);
//}

void *create_and_send_obj(void *arg) {

    auto tuple = (Tuple *) arg;
    auto re_str = tuple->re_str;
    auto library = tuple->library;
    auto name = tuple->func_name;

    int size = re_str.length();
    auto obj = library->create_object(name, true, size + 1);
    auto val = static_cast<char *>(obj->get_value());
    memset(val, 0, size + 1);
    if (re_str.length() > 0) {
        strncpy(val, re_str.c_str(), re_str.length());
    } else {
        memset(val, 0xff, size);
        val[size - 1] = '\0';
    }
    library->send_object(obj);

    delete tuple;
    return nullptr;
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

    std::string re_str = j.dump();

    std::vector<pthread_t> threads;

    std::string fanout_fun_name = "exp06_run_audit";
    for (int i = 1; i <= fanout_num; i++) {
        std::string func_name = fanout_fun_name + "_" + std::to_string(i);
        auto tuple = new Tuple(library, func_name, re_str);

        pthread_t tid;
        pthread_create(&tid, nullptr, create_and_send_obj, (void *) tuple);

        threads.push_back(tid);
    }

    for (auto thread: threads) {
        pthread_join(thread, nullptr);
    }

    return 0;
}
}