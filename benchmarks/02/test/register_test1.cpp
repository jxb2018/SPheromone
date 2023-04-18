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
    funcs.emplace_back("exp02_web_server_0");
    funcs.emplace_back("exp02_upload_text_1");
    funcs.emplace_back("exp02_upload_movieid_1");
    funcs.emplace_back("exp02_upload_uniqueid_1");
    funcs.emplace_back("exp02_upload_rating_2");
    funcs.emplace_back("exp02_upload_text_3");
    funcs.emplace_back("exp02_upload_movieid_3");
    funcs.emplace_back("exp02_upload_uniqueid_3");
    funcs.emplace_back("exp02_upload_rating_3");
    funcs.emplace_back("exp02_store_review_4");

    // dependence (workflow)
    std::vector<TUPLE > dep;
//    dep.push_back({{"exp02_web_server_0"}, {"exp02_register_movieid_1", "exp02_upload_text_1", "exp02_upload_movieid_1", "exp02_upload_uniqueid_1"}, DIRECT});
    dep.push_back({{"exp02_web_server_0"}, {"exp02_upload_text_1", "exp02_upload_movieid_1", "exp02_upload_uniqueid_1"}, DIRECT});
    dep.push_back({{"exp02_upload_text_1"}, {"exp02_upload_text_3"}, DIRECT});
    dep.push_back({{"exp02_upload_uniqueid_1"}, {"exp02_upload_uniqueid_3"}, DIRECT});
    dep.push_back({{"exp02_upload_movieid_1"}, {"exp02_upload_rating_2", "exp02_upload_movieid_3"}, DIRECT});
    dep.push_back({{"exp02_upload_rating_2"}, {"exp02_upload_rating_3"}, DIRECT});
    dep.push_back({{"exp02_upload_text_3", "exp02_upload_movieid_3", "exp02_upload_rating_3", "exp02_upload_uniqueid_3"}, {"exp02_store_review_4"}, MANY_TO_ONE});

    client.register_app(app_name, funcs, dep);

    // -----------------------------------------
    std::string app_name2("exp02_register");
    std::vector<std::string> funcs2;
    funcs2.emplace_back("exp02_register_movieid_1");
    std::vector<TUPLE > dep2;
    client.register_app(app_name2, funcs2, dep2);

    std::cout << "register finished!" << std::endl;

    return 0;
}