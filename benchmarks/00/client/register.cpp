//
// Created by tank on 3/19/23.
//

#include "pheromone_client.h"

int main() {
    auto manager_ip = getenv("MANAGER_IP");
    PheromoneClient client(manager_ip);

    std::string app_name = "exp00";

    std::vector<std::string> funcs;
    funcs.emplace_back("exp00_front");
    funcs.emplace_back("exp00_back");

    // set dependence
    std::vector<TUPLE> dep;
    dep.push_back({{"exp00_front"},{"exp00_back"},DIRECT});
    dep.push_back({{"exp00_back"},{"exp00_back"},DIRECT});

    // register app
    client.register_app(app_name, funcs, dep);

    std::cout << "register finished!" << std::endl;
    return 0;
}