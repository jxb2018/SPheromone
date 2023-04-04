//
// Created by mutuxixi on 23-4-1.
//

#include "cpp_function.hpp"
#include "ComposeReviewHandler.h"
#include <nlohmann/json.hpp>

using namespace media_service;

static ClientPool<MCClient> *g_mc_client_pool;

extern "C" {
int handle(UserLibraryInterface *library, int arg_size, char **arg_values) {

    using json = nlohmann::json;

    init_compose_review(g_mc_client_pool);

    json j = json::parse(arg_values[0]);

    auto unique_id  = new UniqueId();
    unique_id->req_id = j["req_id"];
    unique_id->review_id = j["review_id"];

    auto handler = new ComposeReviewHandler(g_mc_client_pool, library);
    int ret = handler->UploadUniqueId(*unique_id);

    std::cout << "UploadUniqueId3 finished!" << std::endl;

    return ret;
}
}