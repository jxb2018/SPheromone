//
// Created by tank on 3/19/23.
//

#include "pheromone_client.h"
#include <unistd.h>

int main(int argc, char **argv) {
    auto manager_ip = getenv("MANAGER_IP");
    PheromoneClient client(manager_ip);

    // step1: register app

    // app name and func name
    std::string app_name("exp01");

    // funcs in app
    std::vector<std::string> funcs;
    funcs.emplace_back("exp01_frontend");
    funcs.emplace_back("exp01_backend");

    // dependence
    std::vector<TUPLE > dep;
    dep.push_back({{"exp01_frontend"}, {"exp01_backend"}, DIRECT});

    client.register_app(app_name, funcs, dep);

    // step2: create bucket
    std::string bucket_name("b_exp01_backend");
    client.create_bucket(app_name, bucket_name);

    // step3: create trigger
    std::map<std::string, std::string> primitive;
    primitive["function"] = "exp01_backend";
    primitive["key_name"] = "k_exp01_frontend";
    std::string trigger_name("trigger1");
    client.add_trigger(app_name, bucket_name, trigger_name, IMMEDIATE, primitive, 0);

    std::cout << "register finished!" << std::endl;

    return 0;
}