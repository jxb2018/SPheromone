//
// Created by tank on 3/19/23.
//

#include "pheromone_client.h"

int main() {
    PheromoneClient client("127.0.0.1");

    std::string app_name = "exp01";

    std::vector<std::string> funcs;
    funcs.push_back("exp01_frontend");

    client.register_app(app_name, funcs);
    client.call_app(app_name, "exp01_frontend", "");

    std::cout << "finished!" << std::endl;

    return 0;
}