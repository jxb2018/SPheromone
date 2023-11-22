//
// Created by tank on 3/21/23.
//

#include "cpp_function.hpp"
#include "ReviewStorageHandler.h"
#include <nlohmann/json.hpp>
#include <string>
#include "utils_for_test.h"

using namespace media_service;

static mongoc_client_pool_t *g_mongodb_client_pool;
static ClientPool<MCClient> *g_mc_client_pool;

auto status = init_review_storage(g_mongodb_client_pool, g_mc_client_pool);

auto gateway_addr = utils::GetEnvVariable("LUMINE_GATEWAY_ADDR", "");
auto gateway_port = utils::GetEnvVariable("LUMINE_GATEWAY_PORT", "");

auto request = utils::Socket(gateway_addr.c_str(), std::stoi(gateway_port));

extern "C" {
int handle(UserLibraryInterface *library, int arg_size, char **arg_values) {

//    std::cout << "input is:" << std::endl;
//    for (int i = 0; i < arg_size; i++) {
//        std::cout << arg_values[i] << std::endl;
//    }

    auto start_time = utils::get_timestamp_us();
    using json = nlohmann::json;

    json j;
    for (int i = 0; i < arg_size; i++) {
        j.merge_patch(json::parse(arg_values[i]));
    }

    auto review = new Review();
    review->review_id = j["review_id"];
//    review->user_id=j["user_id"];
    review->req_id = j["req_id"];
    strcpy(review->text, j["text"].get<std::string>().c_str());
    strcpy(review->movie_id, j["movie_id"].get<std::string>().c_str());
    review->rating = j["rating"];
//    review->timestamp=j["timestamp"];

//    std::cout << "review_id: " << review->review_id << std::endl;
//    std::cout << "user_id: " << review->user_id << std::endl;
//    std::cout << "req_id: " << review->req_id << std::endl;
//    std::cout << "text: " << review->text << std::endl;
//    std::cout << "movie_id: " << review->movie_id << std::endl;
//    std::cout << "rating: " << review->rating << std::endl;
//    std::cout << "timestamp: " << review->timestamp << std::endl;

    auto handler = new ReviewStorageHandler(g_mc_client_pool, g_mongodb_client_pool);
    int ret = handler->StoreReview(*review);


    if (!gateway_addr.empty() && !gateway_port.empty()) {
        auto request_payload = std::to_string(j["req_id"].get<long>());

        if (request.conn() < 0) {
            perror("Failed to connect");
            return -1;
        }
        request.issue_http_request("POST", "/function/exp09End", request_payload.c_str());
    }

    auto end_time = utils::get_timestamp_us();
    std::cout << "StoreReview4 finished!, token " << end_time - start_time << std::endl;

    return ret;
}
}