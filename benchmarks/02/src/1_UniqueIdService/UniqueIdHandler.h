//
// Created by mutuxixi on 23-4-4.
//

#ifndef SPHEROMONE_UNIQUEIDHANDLER_H
#define SPHEROMONE_UNIQUEIDHANDLER_H

#include <nlohmann/json.hpp>
#include "MediaServiceTypes.h"
#include "cpp_function.hpp"
#include "string"
#include "utils.h"

namespace media_service {
    using json = nlohmann::json;
    using namespace media_service;

    class UniqueIdHandler {
    public:
        explicit UniqueIdHandler(UserLibraryInterface *library)
                : library_(library) {}

        int UploadUniqueId(int64_t req_id);

    private:
        UserLibraryInterface *library_;
    };

    inline int64_t make_unique_id() {
        // timestamp
        struct timespec time_stamp{};
        clock_gettime(CLOCK_REALTIME, &time_stamp);
        uint64_t timestamp = time_stamp.tv_sec * 1000000 + time_stamp.tv_nsec / 1000;

        std::stringstream sstream;
        sstream << std::hex << timestamp;
        std::string timestamp_hex(sstream.str());

        if (timestamp_hex.size() > 10) {
            timestamp_hex.erase(0, timestamp_hex.size() - 10);
        } else if (timestamp_hex.size() < 10) {
            timestamp_hex = std::string(10 - timestamp_hex.size(), '0') + timestamp_hex;
        }

        // Empty the sstream buffer.
        sstream.clear();
        sstream.str(std::string());

        sstream << std::hex << rand();
        std::string counter_hex(sstream.str());

        if (counter_hex.size() > 3) {
            counter_hex.erase(0, counter_hex.size() - 3);
        } else if (counter_hex.size() < 3) {
            counter_hex = std::string(3 - counter_hex.size(), '0') + counter_hex;
        }
        std::string review_id_str = timestamp_hex + counter_hex;
        int64_t review_id = stoul(review_id_str, nullptr, 16) & 0x7FFFFFFFFFFFFFFF;

        return review_id;
    }

    int UniqueIdHandler::UploadUniqueId(int64_t req_id) {

        json j;
        j["req_id"]=req_id;
        j["review_id"]=make_unique_id();

        auto str = j.dump();
        auto obj = library_->create_object("exp02_upload_uniqueid_3", true, 512);

        auto val = (char *) (obj->get_value());
        memset(val, 0, 512);
        strcpy(val, str.c_str());

        library_->send_object(obj);

        return 0;
    }
}

#endif //SPHEROMONE_UNIQUEIDHANDLER_H
