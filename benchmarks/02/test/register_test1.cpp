//
// Created by mutuxixi on 23-3-24.
//
#include "pheromone_client.h"
#include <unistd.h>

int main(int argc, char **argv) {
    auto manager_ip = getenv("MANAGER_IP");
    PheromoneClient client(manager_ip);

    // register app

    // app name and func name
    std::string app_name("exp02");

    // funcs in app
    std::vector<std::string> funcs;
    funcs.emplace_back("exp02_send_review_4");
    funcs.emplace_back("exp02_store_review_4");

    // dependence (workflow)
    std::vector<TUPLE > dep;
    dep.push_back({{"exp02_send_review_4"}, {"exp02_store_review_4"}, DIRECT});

    client.register_app(app_name, funcs, dep);

    std::cout << "register finished!" << std::endl;

    return 0;
}