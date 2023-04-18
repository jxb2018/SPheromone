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
    args.emplace_back("1"); // req_id
    args.emplace_back("Avengers: Endgame"); // title
    args.emplace_back("1"); // rating
    args.emplace_back("111111111111111111111111122222222222222"); //text

    client.call_app(app_name, "exp02_web_server_0", args);

    return 0;
}