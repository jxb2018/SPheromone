//
// Created by tank on 3/19/23.
//

#include "pheromone_client.h"

int main() {
    auto manager_ip = getenv("MANAGER_IP");
    PheromoneClient client(manager_ip);

    // app name
    std::string app_name("exp01");

    // func name
    std::string func_name("exp01_frontend");

    // args
    std::vector<std::string> args;
    args.emplace_back("3"); // function chain length
    args.emplace_back("16B"); // payload size

    // invoke
    client.call_app(app_name, func_name, args);

    std::cout << "invoke finished!" << std::endl;
    return 0;
}