//
// Created by mutuxixi on 3/29/23.
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



    return 0;
}
}