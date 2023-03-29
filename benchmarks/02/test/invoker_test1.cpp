//
// Created by mutuxixi on 23-3-24.
//
#include "pheromone_client.h"
#include <unistd.h>

int main(int argc, char **argv) {
    auto manager_ip = getenv("MANAGER_IP");
    PheromoneClient client(manager_ip);

    // app name and func name
    std::string app_name("exp02");

    // step2: invoke
    std::vector<std::string> args;
//    args.emplace_back(argv[1]); // function chain length, like "4"
//    args.emplace_back(argv[2]); // payload size
    client.call_app(app_name, "exp02_send_review_4", args);

    return 0;
}