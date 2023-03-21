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
    args.push_back("3"); // function chain length
    args.push_back("16B"); // payload size

    std::map<std::string, std::string> primitive;
    primitive["function"] = "exp01_backend";

    std::string bucket_name("b_exp01_backend");

    // only the first invocation executes
    {
        client.register_app(app_name, funcs);
        client.create_bucket(app_name, bucket_name);
        client.add_trigger(app_name, bucket_name, "trigger1", IMMEDIATE, primitive, 0);
    }

    client.call_app(app_name, "exp01_frontend", args);

    std::cout << "finished!" << std::endl;

    return 0;
}