//
// Created by tank on 3/19/23.
//

#include "pheromone_client.h"

int main(int argc, char *argv[]) {
    if (argc != 2) {
        std::cerr << "Usage: " << argv[0] << " <int: fan out num>" << std::endl;
        return 1;
    }
    int fanout_num = std::stoi(argv[1]);

    std::vector<std::string> fanout_target;
    std::string fanout_fun_name="exp06_run_audit";

    for (int i = 1; i <= fanout_num; i++) {
        std::string result = fanout_fun_name + "_" + std::to_string(i);
        fanout_target.push_back(result);
    }


    auto manager_ip = getenv("MANAGER_IP");
    PheromoneClient client(manager_ip);

    std::string app_name = "exp06";

    std::vector<std::string> funcs;
    funcs.emplace_back("exp06_marketdata");
    for (int i = 1; i <= fanout_num; i++) {
        funcs.emplace_back(fanout_target[i-1]);
    }
    funcs.emplace_back("exp06_marginbalance");


    // set dependence
    std::vector<TUPLE> dep;
    dep.push_back({{"exp06_marketdata"},fanout_target,DIRECT});

    dep.push_back({fanout_target,{"exp06_marginbalance"},MANY_TO_ONE});

    // register app
    client.register_app(app_name, funcs, dep);

    std::cout << "register finished!" << std::endl;
    return 0;
}