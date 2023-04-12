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

namespace media_service {

    using json = nlohmann::json;

    class ComposeReviewHandler {
    public:
        explicit ComposeReviewHandler(UserLibraryInterface *library)
                : library_(library) {}

        int UploadText(const Text &val);

        int UploadUniqueId(const UniqueId &val);

        int UploadRating(const Rating &val);

        int UploadMovieId(const MovieId &val);

    private:
        UserLibraryInterface *library_;
    };

    int ComposeReviewHandler::UploadText(const Text &val) {
        json j;
        j["req_id"]=val.req_id;
        j["text"]=val.text;

        auto str = j.dump();
        auto obj = library_->create_object("exp02_store_review_4", true, str.size());
        auto v = static_cast<char *>(obj->get_value());
        strncpy(v, str.c_str(), str.length());

        library_->send_object(obj);

        return 0;
    }

    int ComposeReviewHandler::UploadUniqueId(const UniqueId &val) {
        json j;
        j["review_id"]=val.review_id;
        j["req_id"]=val.req_id;

        auto str = j.dump();
        auto obj = library_->create_object("exp02_store_review_4", true, str.size());
        auto v = static_cast<char *>(obj->get_value());
        strncpy(v, str.c_str(), str.length());

        library_->send_object(obj);

        return 0;
    }

    int ComposeReviewHandler::UploadRating(const Rating &val) {
        json j;
        j["req_id"]=val.req_id;
        j["rating"]=val.rating;

        auto str = j.dump();
        auto obj = library_->create_object("exp02_store_review_4", true, str.size());
        auto v = static_cast<char *>(obj->get_value());
        strncpy(v, str.c_str(), str.length());

        library_->send_object(obj);

        return 0;
    }

    int ComposeReviewHandler::UploadMovieId(const MovieId &val) {
        json j;
        j["req_id"]=val.req_id;
        j["movie_id"]=val.movie_id;

        auto str = j.dump();
        auto obj = library_->create_object("exp02_store_review_4", true, str.size());
        auto v = static_cast<char *>(obj->get_value());
        strncpy(v, str.c_str(), str.length());

        library_->send_object(obj);

        return 0;
    }

}

#endif //SPHEROMONE_COMPOSEREVIEWHANDLER_H