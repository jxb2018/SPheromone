//
// Created by tank on 3/19/23.
//

#include "pheromone_client.h"

int main(int argc, char *argv[]) {
    auto manager_ip = getenv("MANAGER_IP");
    PheromoneClient client(manager_ip);

    std::string app_name = "exp06";

//    if(argc != 3){
//        std::cerr << "bad input!" << std::endl;
//        return -1;
//    }
    // args
    std::vector<std::string> args;
    args.emplace_back(argv[1]); // function chain length
//    args.emplace_back(argv[2]); // payload size

    // invoke
    client.call_app(app_name, "exp06_marketdata", args);

//    std::cout << "invoke finished!" << std::endl;

    return 0;
}