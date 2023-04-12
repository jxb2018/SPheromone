//
// Created by mutuxixi on 23-4-1.
//

#ifndef SPHEROMONE_TEXTHANDLER_H
#define SPHEROMONE_TEXTHANDLER_H

#include <nlohmann/json.hpp>
#include "MediaServiceTypes.h"
#include "cpp_function.hpp"
#include "string"
#include "utils.h"

namespace media_service {
    using json = nlohmann::json;
    using namespace media_service;

    class TextHandler {
    public:
        explicit TextHandler(UserLibraryInterface *library)
                : library_(library) {}

        int UploadText(Text &val);

    private:
        UserLibraryInterface *library_;
    };

    int TextHandler::UploadText(Text &text) {

        json j;
        j["req_id"]=text.req_id;
        j["text"]=text.text;

        auto str = j.dump();
        auto obj = library_->create_object("exp02_upload_text_3", true, 1024);

        auto val = (char *) (obj->get_value());
        memset(val, 0, 1024);
        strcpy(val, str.c_str());

        library_->send_object(obj);

        return 0;
    }
}

#endif //SPHEROMONE_TEXTHANDLER_H
