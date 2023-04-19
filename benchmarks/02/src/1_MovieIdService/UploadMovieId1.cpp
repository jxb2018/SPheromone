//
// Created by mutuxixi on 23-4-3.
//

#include "cpp_function.hpp"
#include "MovieIdHandler.h"
#include <nlohmann/json.hpp>
#include <string>

using namespace media_service;

static mongoc_client_pool_t *g_mongodb_client_pool;
static ClientPool<MCClient> *g_mc_client_pool;

auto status = init_movie_id(g_mongodb_client_pool, g_mc_client_pool);

extern "C" {
int handle(UserLibraryInterface *library, int arg_size, char **arg_values) {

    using json = nlohmann::json;

    json j = json::parse(arg_values[0]);

    auto title = new Title();
    title->req_id=j["req_id"];
    title->rating=j["rating"];
    strcpy(title->title, j["title"].get<std::string>().c_str());

    auto handler = new MovieIdHandler(library, g_mc_client_pool, g_mongodb_client_pool);
    int ret = handler->UploadMovieId(*title);

    std::cout << "UploadMovieId1 finished!" << std::endl;

    return ret;
}
}