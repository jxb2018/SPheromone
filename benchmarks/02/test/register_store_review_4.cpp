//
// Created by mutuxixi on 23-3-24.
//
#include "pheromone_client.h"
#include <unistd.h>

int main(int argc, char **argv) {
    auto manager_ip = getenv("MANAGER_IP");
    PheromoneClient client(manager_ip);

    // step1: register app

    // app name and func name
    std::string app_name("exp02");

    // funcs in app
    std::vector<std::string> funcs;
    funcs.emplace_back("exp02_send_review_4");
    funcs.emplace_back("exp02_store_review_4");

    // dependence
    std::vector<TUPLE > dep;
    dep.push_back({{"exp02_send_review_4"}, {"exp02_store_review_4"}, DIRECT});

    client.register_app(app_name, funcs, dep);

    // step2: create bucket
    std::string bucket_name("b_exp02_store_review_4");
//    client.create_bucket(app_name, bucket_name);

    // step3: create trigger
    std::map<std::string, std::string> primitive;
    primitive["function"] = "exp02_store_review_4";
    primitive["key_name"] = "exp02_send_review_4";
    std::string trigger_name("trigger2");
//    client.add_trigger(app_name, bucket_name, trigger_name, IMMEDIATE, primitive, 0);

    std::cout << "register finished!" << std::endl;

    return 0;
}