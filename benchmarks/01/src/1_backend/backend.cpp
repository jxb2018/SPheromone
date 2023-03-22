//
// Created by tank on 3/19/23.
//
#include "iostream"
#include "cpp_function.hpp"
#include "utils.h"

using namespace exp01;

extern "C" {
int handle(UserLibraryInterface *library, int arg_size, char **arg_values) {

    std::string val = arg_values[0];

    std::string start_time_str(val.c_str(), 16);
    long start_time;
    try{
        start_time = std::stol(start_time_str);
    }catch (std::exception &e){
        std::cerr << "exp01_backend: invalid input!, " << start_time_str << std::endl;
        return -1;
    }
    long end_time = exp01::get_timestamp_us();
    std::cout << "transfer payload, size = " << strlen(arg_values[0]) << ", token " << end_time - start_time << std::endl;
    return 0;
}
}