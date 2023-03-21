//
// Created by tank on 2/10/23.
//

#ifndef LUMINE_EXPERIMENTS_MEDIASERVICETYPES_H
#define LUMINE_EXPERIMENTS_MEDIASERVICETYPES_H

#include <cstdint>

#define REVIEW_SIZE_LIMIT 860
namespace media_service {
    struct Review {
        int64_t review_id;
        int64_t user_id;
        int64_t req_id;
        char text[512];
        char movie_id[128];
        int64_t rating;
        int64_t timestamp;
    };

    static_assert(sizeof(Review) <= REVIEW_SIZE_LIMIT, "illegal review size");

    struct Text {
        int64_t req_id;
        char text[512];
    };

    struct TitleForRegister{
        int64_t req_id;
        char title[128]; // movie title
        char movie_id[128];
    };

    struct Title {
        int64_t req_id;
        char title[128]; // movie title
        int64_t rating;
    };

    struct MovieId {
        int64_t req_id;
        char movie_id[128];
        int64_t rating;
    };

    struct UniqueId {
        int64_t req_id;
        int64_t review_id;
    };

    struct Rating {
        int64_t req_id;
        int64_t rating;
    };


}

#endif //LUMINE_EXPERIMENTS_MEDIASERVICETYPES_H
