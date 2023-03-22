//
// Created by tank on 3/19/23.
//

#include "pheromone_client.h"

int main() {
    auto manager_ip = getenv("MANAGER_IP");
    PheromoneClient client(manager_ip);

    std::string app_name = "exp00";

    // set function name
    std::string func_name("exp00_inc");

    std::vector<std::string> funcs;
    funcs.emplace_back(func_name);

//    std::map<std::string, std::string> primitive;
//    primitive["function"] = func_name;

    // will be used by BY_SET
//    primitive["key_name"] = "k_exp01_frontend";

    // set bucket name
//    std::string bucket_name("b_exp01_backend");



    // set trigger name
//    std::string trigger_name("trigger_by_set");

    // set dependence
    std::vector<TUPLE> dep;

    dep.push_back({{"exp00_inc"},{"exp00_inc"},DIRECT});

    // step1: register app
    client.register_app(app_name, funcs, dep);

    // step2: create bucket
//    client.create_bucket(app_name, bucket_name);

    // step3: add trigger
//    client.add_trigger(app_name, bucket_name, trigger_name, IMMEDIATE, primitive, 0);
    //client.add_trigger(app_name, bucket_name, trigger_name, BY_SET, primitive, 0);

    // args
    std::vector<std::string> args;
    args.emplace_back("3"); // function chain length
    args.emplace_back("16B"); // payload size

    // invoke
    client.call_app(app_name, func_name, args);

    std::cout << "invoke finished!" << std::endl;

    return 0;
}