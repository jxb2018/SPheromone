//
// Created by tank on 3/19/23.
//

#include "pheromone_client.h"

int main() {
    PheromoneClient client("127.0.0.1");

    std::string app_name = "exp01";

    std::vector<std::string> funcs;
    funcs.emplace_back("exp01_frontend");

    std::map<std::string, std::string> primitive;
    primitive["function"] = "exp01_backend";

    // will be used by BY_SET
    primitive["key_name"] = "k_exp01_frontend";

    // set bucket name
    std::string bucket_name("b_exp01_backend");

    // set trigger name
    std::string trigger_name("trigger_by_set");

    // step1: register app
    client.register_app(app_name, funcs);

    // step2: create bucket
    client.create_bucket(app_name, bucket_name);

    // step3: add trigger
    client.add_trigger(app_name, bucket_name, trigger_name, IMMEDIATE, primitive, 0);
    //client.add_trigger(app_name, bucket_name, trigger_name, BY_SET, primitive, 0);

    std::cout << "register finished!" << std::endl;

    return 0;
}