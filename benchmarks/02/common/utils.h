#ifndef MEDIA_MICROSERVICES_UTILS_H
#define MEDIA_MICROSERVICES_UTILS_H

#include <string>
#include <fstream>
#include <iostream>
#include <nlohmann/json.hpp>
#include <utility>

#include "logger.h"

namespace media_service {
    using json = nlohmann::json;

    inline std::string GetEnvVariable(std::string_view name,
                                      std::string default_value = "") {
        char *value = getenv(std::string(name).c_str());
        return value != nullptr ? value : std::move(default_value);
    }

    int load_config_file(const std::string &file_name, json *config_json) {
        std::ifstream json_file;
        json_file.open(file_name);
        if (json_file.is_open()) {
            json_file >> *config_json;
            json_file.close();
            return 0;
        } else {
            LOG(error) << "Cannot open service-config.json";
            return -1;
        }
    };

    int load_config(json *config_json) {
        auto config_path = GetEnvVariable("STORE_SERVICE_CONFIG_PATH", "");
        return load_config_file(config_path, config_json);
    }

    static const std::string base64_chars =
            "ABCDEFGHIJKLMNOPQRSTUVWXYZ"
            "abcdefghijklmnopqrstuvwxyz"
            "0123456789+/";


    static inline bool is_base64(unsigned char c) {
        return (isalnum(c) || (c == '+') || (c == '/'));
    }

    std::string base64_encode(const std::string &in) {
        std::string ret;
        int i = 0;
        int j = 0;
        unsigned char char_array_3[3];
        unsigned char char_array_4[4];

        char const *bytes_to_encode = in.data();
        unsigned int in_len = in.length();

        while (in_len--) {
            char_array_3[i++] = *(bytes_to_encode++);
            if (i == 3) {
                char_array_4[0] = (char_array_3[0] & 0xfc) >> 2;
                char_array_4[1] = ((char_array_3[0] & 0x03) << 4) + ((char_array_3[1] & 0xf0) >> 4);
                char_array_4[2] = ((char_array_3[1] & 0x0f) << 2) + ((char_array_3[2] & 0xc0) >> 6);
                char_array_4[3] = char_array_3[2] & 0x3f;

                for (i = 0; (i < 4); i++)
                    ret += base64_chars[char_array_4[i]];
                i = 0;
            }
        }

        if (i) {
            for (j = i; j < 3; j++)
                char_array_3[j] = '\0';

            char_array_4[0] = (char_array_3[0] & 0xfc) >> 2;
            char_array_4[1] = ((char_array_3[0] & 0x03) << 4) + ((char_array_3[1] & 0xf0) >> 4);
            char_array_4[2] = ((char_array_3[1] & 0x0f) << 2) + ((char_array_3[2] & 0xc0) >> 6);

            for (j = 0; (j < i + 1); j++)
                ret += base64_chars[char_array_4[j]];

            while ((i++ < 3))
                ret += '=';

        }

        return ret;

    }

} //namespace media_service

#endif //MEDIA_MICROSERVICES_UTILS_H
