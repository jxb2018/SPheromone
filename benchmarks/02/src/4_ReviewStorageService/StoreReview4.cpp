//
// Created by tank on 3/21/23.
//

#include "cpp_function.hpp"
#include "ReviewStorageHandler.h"
#include <nlohmann/json.hpp>
#include <string>

using namespace media_service;

static mongoc_client_pool_t *g_mongodb_client_pool;
static ClientPool<MCClient> *g_mc_client_pool;

extern "C" {
int handle(UserLibraryInterface *library, int arg_size, char **arg_values) {

    using json = nlohmann::json;

    init_review_storage(g_mongodb_client_pool, g_mc_client_pool);

    json j = json::parse(arg_values[0]);

    auto review = new Review();
    review->review_id=j["review_id"];
    review->user_id=j["user_id"];
    review->req_id=j["req_id"];
    strcpy(review->text, j["text"].get<std::string>().c_str());
    strcpy(review->movie_id, j["movie_id"].get<std::string>().c_str());
    review->rating=j["rating"];
    review->timestamp=j["timestamp"];

    std::cout << "review_id: " << review->review_id << std::endl;
    std::cout << "user_id: " << review->user_id << std::endl;
    std::cout << "req_id: " << review->req_id << std::endl;
    std::cout << "text: " << review->text << std::endl;
    std::cout << "movie_id: " << review->movie_id << std::endl;
    std::cout << "rating: " << review->rating << std::endl;
    std::cout << "timestamp: " << review->timestamp << std::endl;

    auto handler = new ReviewStorageHandler(g_mc_client_pool, g_mongodb_client_pool);
    int ret = handler->StoreReview(*review);

    std::cout << "StoreReview4 finished!" << std::endl;

    return ret;
}
}