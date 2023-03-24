//
// Created by tank on 3/21/23.
//

#include "cpp_function.hpp"
#include "ReviewStorageHandler.h"

using namespace media_service;

static mongoc_client_pool_t *g_mongodb_client_pool;
static ClientPool<MCClient> *g_mc_client_pool;

extern "C" {
int handle(UserLibraryInterface *library, int arg_size, char **arg_values) {

    init_review_storage(g_mongodb_client_pool, g_mc_client_pool);

    EpheObject *obj;
    obj = library->get_object("b_exp02_store_review_4", "k_exp02_send_review_4", true);

    auto val = (char *) (obj->get_value());

    auto review = reinterpret_cast<const Review *>(val);

    auto handler = new ReviewStorageHandler(g_mc_client_pool, g_mongodb_client_pool);
    int ret = handler->StoreReview(*review);

    std::cout << "finished!" << std::endl;

    return ret;
}
}