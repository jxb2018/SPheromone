//
// Created by tank on 3/19/23.
//

#include "iostream"
#include "cpp_function.hpp"

extern "C" {
int handle(UserLibraryInterface *library, int arg_size, char **arg_values) {
    std::cout << "front-end\n";
    return 0;
}
}