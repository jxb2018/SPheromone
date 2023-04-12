//
// Created by mutuxixi on 23-4-3.
//

#include "cpp_function.hpp"
#include "RatingHandler.h"
#include <nlohmann/json.hpp>
#include <string>

using namespace media_service;

static ClientPool<RedisClient> *g_redis_client_pool;

extern "C" {
int handle(UserLibraryInterface *library, int arg_size, char **arg_values) {

    using json = nlohmann::json;

    init_rating(g_redis_client_pool);

    json j = json::parse(arg_values[0]);

    auto movie_id  = new MovieId();
    movie_id->req_id = j["req_id"];
    movie_id->rating = j["rating"];
    strcpy(movie_id->movie_id, j["movie_id"].get<std::string>().c_str());

    auto handler = new RatingHandler(library, g_redis_client_pool);
    int ret = handler->UploadRating(*movie_id);

    std::cout << "UploadRating2 finished!" << std::endl;

    return ret;
}
}