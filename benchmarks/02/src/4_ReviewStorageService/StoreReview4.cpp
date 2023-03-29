//
// Created by tank on 3/21/23.
//

#include "cpp_function.hpp"
#include "ReviewStorageHandler.h"
#include <nlohmann/json.hpp>

using namespace media_service;

static mongoc_client_pool_t *g_mongodb_client_pool;
static ClientPool<MCClient> *g_mc_client_pool;

extern "C" {
int handle(UserLibraryInterface *library, int arg_size, char **arg_values) {

    using json = nlohmann::json;

    init_review_storage(g_mongodb_client_pool, g_mc_client_pool);

    json j = json::parse(arg_values[0]);
    std::cout << j << std::endl;

    Review *review;
    review->review_id=j["review_id"];
    review->user_id=j["user_id"];
    review->req_id=j["req_id"];
    strcpy(review->text, ((std::string)j["text"]).c_str());
    strcpy(review->movie_id, ((std::string)j["movie_id"]).c_str());
    review->rating=j["rating"];
    review->timestamp=j["timestamp"];

    auto handler = new ReviewStorageHandler(g_mc_client_pool, g_mongodb_client_pool);
    int ret = handler->StoreReview(*review);

    std::cout << "finished!" << std::endl;

    return ret;
}
}