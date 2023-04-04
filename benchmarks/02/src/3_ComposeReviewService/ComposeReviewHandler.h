//
// Created by mutuxixi on 3/29/23.
//

#ifndef SPHEROMONE_COMPOSEREVIEWHANDLER_H
#define SPHEROMONE_COMPOSEREVIEWHANDLER_H

#include "MediaServiceTypes.h"
#include "utils_mongodb.h"
#include "utils_memcached.h"
#include "utils.h"
#include "cpp_function.hpp"
#include <nlohmann/json.hpp>

//#define NUM_COMPONENTS 5
#define NUM_COMPONENTS 4
#define MMC_EXP_TIME 10

namespace media_service {

    using json = nlohmann::json;

    enum UploadOp {
        UploadText = 0,
        UploadUniqueId,
        UploadMovieId,
        UploadUserId,
        UploadRating/**/
    };

    const char *UploadOpName[] = {
            "Text",
            "UniqueId",
            "MovieId",
            "UserId",
            "Rating"
    };

    int init_compose_review(ClientPool<MCClient> *&mc_client_pool) {
        json config_json;
        // step 01: init logger
        init_logger();
        // step 02: loca config
        if (load_config(&config_json) != 0)
            return -1;
        mc_client_pool = init_memcached_client_pool(config_json, "compose-review", MONGODB_POOL_MAX_SIZE);
        if (mc_client_pool == nullptr) {
            return -1;
        }
        return 0;
    }

    class ComposeReviewHandler {
    public:
        explicit ComposeReviewHandler(ClientPool<MCClient> *mc_client_pool, UserLibraryInterface *library)
                : mc_client_pool_(mc_client_pool), library_(library) {}

        int ComposeAndUpload(int64_t req_id);

        int UploadText(const Text &val);

        int UploadUniqueId(const UniqueId &val);

        int UploadRating(const Rating &val);

        int UploadMovieId(const MovieId &val);

    private:
        int Upload(const Review &val, UploadOp op);

        ClientPool<MCClient> *mc_client_pool_;
        UserLibraryInterface *library_;
    };

    int ComposeReviewHandler::ComposeAndUpload(int64_t req_id) {
        std::string key_unique_id = std::to_string(req_id) + ":" + UploadOpName[UploadOp::UploadUniqueId];
        std::string key_movie_id = std::to_string(req_id) + ":" + UploadOpName[UploadOp::UploadMovieId];
//        std::string key_user_id = std::to_string(req_id) + ":" + UploadOpName[UploadOp::UploadUserId];
        std::string key_text = std::to_string(req_id) + ":" + UploadOpName[UploadOp::UploadText];
        std::string key_rating = std::to_string(req_id) + ":" + UploadOpName[UploadOp::UploadRating];

        const char *keys[NUM_COMPONENTS] = {
                key_unique_id.c_str(),
                key_movie_id.c_str(),
//                key_user_id.c_str(),
                key_text.c_str(),
                key_rating.c_str()
        };

        // Compose a review from the components obtained from memcached
        auto mc_client = mc_client_pool_->Pop();
        if (!mc_client) {
            fprintf(stderr, "E ComposeAndUpload: no available mc-client\n");
            return -1;
        }

        Review new_review{};
        bool found;
        std::string return_value;
        uint32_t flags;
        for (auto &key: keys) {
            bool success = mc_client->Get(key, &found, &return_value, &flags);
            if (!success) {
                mc_client_pool_->Push(mc_client);
                fprintf(stderr, "E ComposeAndUpload: Cannot get components of request[%ld]\n", req_id);
                return -1;
            }
            if (!found) continue;
            if (key == key_unique_id) {
                new_review.review_id = (int64_t) std::stoul(return_value);
            } else if (key == key_movie_id) {
                strcpy(new_review.movie_id, return_value.c_str());
            } else if (key == key_text) {
                strcpy(new_review.text, return_value.c_str());
//            } else if (key == key_user_id) {
//                new_review.user_id = (int64_t) std::stoul(return_value);
            } else if (key == key_rating) {
                new_review.rating = std::stoi(return_value);
            }
        }

        mc_client_pool_->Push(mc_client);

        json j;
        j["review_id"]=new_review.review_id;
        j["user_id"]=new_review.user_id;
        j["req_id"]=new_review.req_id;
        j["text"]=new_review.text;
        j["movie_id"]=new_review.movie_id;
        j["rating"]=new_review.rating;
        j["timestamp"]=new_review.timestamp;

        auto str = j.dump();
        auto obj = library_->create_object("exp02_store_review_4", true, 2048);

        auto val = (char *) (obj->get_value());
        memset(val, 0, 2048);
        strcpy(val, str.c_str());

        library_->send_object(obj);

        return 0;
    }

    int ComposeReviewHandler::Upload(const Review &val, UploadOp op) {
        auto req_id = val.req_id;
        std::string key_counter = std::to_string(req_id) + ":counter";
        auto mc_client = mc_client_pool_->Pop();
        if (mc_client == nullptr) {
            fprintf(stderr, "E Upload%s: no available mc-client\n", UploadOpName[op]);
            return -1;
        }

        bool stored, found, success;
        uint32_t flags;
        // Initialize the counter to 0 if there is not in the memcached
        success = mc_client->Add(key_counter, "0", 0, MMC_EXP_TIME, &stored);

        // error if it cannot be stored
        if (!success) {
            mc_client_pool_->Push(mc_client);
            fprintf(stderr, "E Upload%s: Failed to initialize the counter for request[%ld]\n", UploadOpName[op],
                    req_id);
            return -1;
        }

        // store text to memcached
        uint64_t counter_value = 0;
        std::string key = std::to_string(req_id) + ":" + UploadOpName[op];
        switch (op) {
            case UploadOp::UploadText:
                success = mc_client->Add(key, val.text, 0, MMC_EXP_TIME, &stored);
                break;
            case UploadOp::UploadUniqueId:
                success = mc_client->Add(key, std::to_string(val.review_id), 0, MMC_EXP_TIME, &stored);
                break;
            case UploadOp::UploadRating:
                success = mc_client->Add(key, std::to_string(val.rating), 0, MMC_EXP_TIME, &stored);
                break;
            case UploadOp::UploadMovieId:
                success = mc_client->Add(key, val.movie_id, 0, MMC_EXP_TIME, &stored);
            default:
                break;
        }

        if (!success) {
            mc_client_pool_->Push(mc_client);
            fprintf(stderr, "E Upload%s: Cannot store %s of request[%ld]\n", UploadOpName[op], UploadOpName[op],
                    req_id);
            return -1;
        } else if (!stored) {
            // Another thread has uploaded text, which is an unexpected behaviour.
            std::string counter_value_str;
            success = mc_client->Get(key_counter, &found, &counter_value_str, &flags);
            fprintf(stderr, "W Upload%s: %s of request[%ld] has already been stored\n", UploadOpName[op],
                    UploadOpName[op], req_id);
            if (!success || !found) {
                mc_client_pool_->Push(mc_client);
                fprintf(stderr, "E Upload%s: Cannot get the counter of request[%ld]\n", UploadOpName[op], req_id);
                return -1;
            }
            counter_value = std::stoul(counter_value_str);
        } else {
            // Atomically increment and get the counter value
            success = mc_client->Incr(key_counter, 1, &counter_value);
            if (!success) {
                mc_client_pool_->Push(mc_client);
                fprintf(stderr, "E Upload%s: Cannot increment and get the counter of request[%ld]\n", UploadOpName[op],
                        req_id);
                return -1;
            }
        }

        mc_client_pool_->Push(mc_client);

        // If this thread is the last one uploading the review components,
        // it is in charge of compose the request and upload to the microservices in
        // the next tier.
        std::cout<<"counter_value: "<<counter_value<<std::endl;
        if (counter_value == NUM_COMPONENTS) {
            ComposeAndUpload(req_id);
        }

        return 0;
    }

    int ComposeReviewHandler::UploadText(const Text &val) {
//        Review review{};
//        review.req_id = val.req_id;
//        strcpy(review.text, val.text);

        json j;
        j["req_id"]=val.req_id;
        j["text"]=val.text;

        auto str = j.dump();
        auto obj = library_->create_object("exp02_store_review_4", true, 2048);

        auto v = (char *) (obj->get_value());
        memset(v, 0, 2048);
        strcpy(v, str.c_str());

        library_->send_object(obj);

//        return Upload(review, UploadOp::UploadText);
        return 0;
    }

    int ComposeReviewHandler::UploadUniqueId(const UniqueId &val) {
//        Review review = {
//                .review_id = val.review_id,
//                .req_id = val.req_id
//        };

        json j;
        j["review_id"]=val.review_id;
        j["req_id"]=val.req_id;

        auto str = j.dump();
        auto obj = library_->create_object("exp02_store_review_4", true, 2048);

        auto v = (char *) (obj->get_value());
        memset(v, 0, 2048);
        strcpy(v, str.c_str());

        library_->send_object(obj);

//        return Upload(review, UploadOp::UploadUniqueId);
        return 0;
    }

    int ComposeReviewHandler::UploadRating(const Rating &val) {
//        Review review = {
//                .req_id = val.req_id,
//                .rating = val.rating
//        };
//        return Upload(review, UploadOp::UploadRating);
        json j;
        j["req_id"]=val.req_id;
        j["rating"]=val.rating;

        auto str = j.dump();
        auto obj = library_->create_object("exp02_store_review_4", true, 2048);

        auto v = (char *) (obj->get_value());
        memset(v, 0, 2048);
        strcpy(v, str.c_str());

        library_->send_object(obj);

        return 0;
    }

    int ComposeReviewHandler::UploadMovieId(const MovieId &val) {
//        Review review{};
//        review.req_id = val.req_id;
//        strcpy(review.movie_id, val.movie_id);
//        return Upload(review, UploadOp::UploadMovieId);
        json j;
        j["req_id"]=val.req_id;
        j["movie_id"]=val.movie_id;

        auto str = j.dump();
        auto obj = library_->create_object("exp02_store_review_4", true, 2048);

        auto v = (char *) (obj->get_value());
        memset(v, 0, 2048);
        strcpy(v, str.c_str());

        library_->send_object(obj);
        return 0;
    }

}

#endif //SPHEROMONE_COMPOSEREVIEWHANDLER_H