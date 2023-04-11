//
// Created by mutuxixi on 23-4-1.
//

#include "cpp_function.hpp"
#include "ComposeReviewHandler.h"
#include <nlohmann/json.hpp>

using namespace media_service;

extern "C" {
int handle(UserLibraryInterface *library, int arg_size, char **arg_values) {

    using json = nlohmann::json;

    json j = json::parse(arg_values[0]);

    auto movie_id  = new MovieId();
    movie_id->req_id = j["req_id"];
//    movie_id->rating = j["rating"];
    strcpy(movie_id->movie_id, j["movie_id"].get<std::string>().c_str());

    auto handler = new ComposeReviewHandler(library);
    int ret = handler->UploadMovieId(*movie_id);

    std::cout << "UploadMovieId3 finished!" << std::endl;

    return ret;
}
}