//
// Created by tank on 3/19/23.
//

#include "pheromone_client.h"

int main() {
    PheromoneClient client("127.0.0.1");

    std::string app_name = "exp01";

    std::vector<std::string> funcs;
    funcs.push_back("exp01_frontend");

    std::vector<std::string> args;
    args.push_back("1"); // function chain length
    args.push_back("16B"); // payload size

    std::map<std::string, std::string> primitive;
    primitive["function"] = "exp01_backend";

    client.register_app(app_name, funcs);
    client.add_trigger(app_name, "b_exp01_backend", "trigger1", IMMEDIATE, primitive, 0);
    client.call_app(app_name, "exp01_frontend", args);

    std::cout << "finished!" << std::endl;

    return 0;
}