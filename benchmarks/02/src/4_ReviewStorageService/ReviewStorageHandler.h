//
// Created by tank on 2/10/23.
//

#ifndef SPHEROMONE_REVIEWSTORAGEHANDLER_H
#define SPHEROMONE_REVIEWSTORAGEHANDLER_H

#include "MediaServiceTypes.h"
#include "utils_mongodb.h"
#include "utils_memcached.h"
#include "utils.h"
#include "ClientPool.h"
#include "MCClient.h"

namespace media_service {
    int init_review_storage(mongoc_client_pool_t *&mongoc_client_pool, ClientPool<MCClient> *&mc_client_pool) {
        json config_json;

        // step 01: init logger
        init_logger();

        // step 02: loca config
        if (load_config(&config_json) != 0)
            return -1;

        // step 03: set memcache & mongodb client pool
        mc_client_pool = init_memcached_client_pool(config_json, "review-storage", MONGODB_POOL_MAX_SIZE);
        mongoc_client_pool = init_mongodb_client_pool(config_json, "review-storage", MONGODB_POOL_MAX_SIZE);
        if (mongoc_client_pool == nullptr ||
            mc_client_pool == nullptr) {
            return -1;
        }

        return 0;
    }

    class ReviewStorageHandler {
    public:
        ReviewStorageHandler(ClientPool<MCClient> *mc_client_pool,
                             mongoc_client_pool_t *mongodb_client_pool)
                : mc_client_pool_(mc_client_pool),
                  mongodb_client_pool_(mongodb_client_pool) {}

        ~ReviewStorageHandler() = default;

        int StoreReview(const Review &review);

    private:
        ClientPool<MCClient> *mc_client_pool_;
        mongoc_client_pool_t *mongodb_client_pool_;
    };

    int ReviewStorageHandler::StoreReview(const Review &review) {
        // get a mongoc client
        auto mongodb_client = mongoc_client_pool_pop(mongodb_client_pool_);
        if (mongodb_client == nullptr) {
            fprintf(stderr, "no available mongodb client in pool");
            return -1;
        }

        // get collection
        auto collection = mongoc_client_get_collection(mongodb_client, "review", "review");
        if (collection == nullptr) {
            fprintf(stderr, "review collection does not exist");
            mongoc_client_pool_push(mongodb_client_pool_, mongodb_client);
            return -1;
        }

        // prepare data
        bson_t *new_doc = bson_new();
        BSON_APPEND_INT64(new_doc, "review_id", review.review_id);
        BSON_APPEND_INT64(new_doc, "timestamp", review.timestamp);
        BSON_APPEND_INT64(new_doc, "user_id", review.user_id);
        BSON_APPEND_UTF8(new_doc, "movie_id", review.movie_id);
        BSON_APPEND_UTF8(new_doc, "text", review.text);
        BSON_APPEND_INT64(new_doc, "rating", review.rating);
        BSON_APPEND_INT64(new_doc, "req_id", review.req_id);

        // insert
        bson_error_t error;
        bool status = mongoc_collection_insert_one(collection, new_doc, nullptr, nullptr, &error);
        if (!status) {
            bson_destroy(new_doc);
            mongoc_collection_destroy(collection);
            mongoc_client_pool_push(mongodb_client_pool_, mongodb_client);
            return -1;
        }

        bson_destroy(new_doc);
        mongoc_collection_destroy(collection);
        mongoc_client_pool_push(mongodb_client_pool_, mongodb_client);

        return 0;
    }

}

#endif //SPHEROMONE_REVIEWSTORAGEHANDLER_H
