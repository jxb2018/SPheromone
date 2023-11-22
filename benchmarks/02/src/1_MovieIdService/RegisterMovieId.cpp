//
// Created by mutuxixi on 23-4-3.
//

#include "cpp_function.hpp"
#include "MovieIdHandler.h"
#include <nlohmann/json.hpp>
#include <string>
#include "utils_for_test.h"

using namespace media_service;

static mongoc_client_pool_t *g_mongodb_client_pool;
static ClientPool<MCClient> *g_mc_client_pool;

extern "C" {
int handle(UserLibraryInterface *library, int arg_size, char **arg_values) {

    auto start_time = utils::get_timestamp_us();

    using json = nlohmann::json;

    init_movie_id(g_mongodb_client_pool, g_mc_client_pool);

    json j = json::parse(arg_values[0]);

    auto val = new TitleForRegister();
    val->req_id=j["req_id"];
    strcpy(val->title, j["title"].get<std::string>().c_str());
    strcpy(val->movie_id, j["movie_id"].get<std::string>().c_str());

    auto handler = new MovieIdHandler(library, g_mc_client_pool, g_mongodb_client_pool);
    int ret = handler->RegisterMovieId(*val);

    auto end_time = utils::get_timestamp_us();
    std::cout << "RegisterMovieId finished!, token " << end_time - start_time << std::endl;

    return ret;
}
}