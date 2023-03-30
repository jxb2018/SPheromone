//
// Created by tank on 3/19/23.
//

#include "pheromone_client.h"

int main() {
    auto manager_ip = getenv("MANAGER_IP");
    PheromoneClient client(manager_ip);

    std::string app_name = "exp05";

    std::vector<std::string> funcs;
    funcs.emplace_back("exp05_front");
    funcs.emplace_back("exp05_marketdata");
    funcs.emplace_back("exp05_lastpx");
    funcs.emplace_back("exp05_side");
    funcs.emplace_back("exp05_trddate");
    funcs.emplace_back("exp05_volume");
    funcs.emplace_back("exp05_back");

    // set dependence
    std::vector<TUPLE> dep;
    dep.push_back({{"exp05_front"},{"exp05_marketdata","exp05_lastpx","exp05_side","exp05_trddate","exp05_volume"},DIRECT});
//    dep.push_back({{"exp05_front"},{"exp05_marketdata"},DIRECT});
//    dep.push_back({{"exp05_front"},{"exp05_side"},DIRECT});
//    dep.push_back({{"exp05_front"},{"exp05_trddate"},DIRECT});
//    dep.push_back({{"exp05_front"},{"exp05_volume"},DIRECT});

    dep.push_back({{"exp05_marketdata","exp05_lastpx","exp05_side","exp05_trddate","exp05_volume"},{"exp05_marginbalance"},MANY_TO_ONE});

    // register app
    client.register_app(app_name, funcs, dep);

    std::cout << "register finished!" << std::endl;
    return 0;
}