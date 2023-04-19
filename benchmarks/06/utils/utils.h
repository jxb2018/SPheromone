//
// Created by tank on 3/19/23.
//

#ifndef SPHEROMONE_UTILS_H
#define SPHEROMONE_UTILS_H

#include <sys/types.h>
#include <ctime>
#include <arpa/inet.h>
#include <cstdio>
#include <cstring>
#include <sys/socket.h>
#include <unistd.h>
#include <string>
#include <netinet/in.h>
#include <netinet/tcp.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <assert.h>

namespace exp05 {

    inline void fill(char *val, long len) {
        memset(val, 0xff, len);
        val[len - 1] = '\0';
    }

    inline void
    set_payload(char *serialized, int total_chain_len, int cur_chain_len, long payload_size, long send_time) {

        std::string val = std::to_string(total_chain_len) + ","
                          + std::to_string(cur_chain_len) + ","
                          + std::to_string(payload_size) + ","
                          + std::to_string(send_time) + ",#";

        strncpy(serialized, val.c_str(), val.length());

    }

    inline int
    get_payload(char *serialized, int &total_chain_len, int &cur_chain_len, long &payload_size, long &send_time) {
        char *remain;
        char *token = strtok_r(serialized, ",", &remain);

        char payloads[8][64];
        int cnt = 0;

        while (token != nullptr) {
            if (token[0] == '#') {
                break;
            }
            strcpy(payloads[cnt++], token);
            token = strtok_r(nullptr, ",", &remain);
        }

        if (cnt != 4) {
            std::cerr << "bad serialized" << std::endl;
            return -1;
        }

        try {
            total_chain_len = std::stoi(payloads[0]);
            cur_chain_len = std::stoi(payloads[1]);
            payload_size = std::stol(payloads[2]);
            send_time = std::stol(payloads[3]);
        } catch (std::exception &e) {
            std::cerr << "occur error, execute stoi" << std::endl;
            return -1;
        }
        return 0;
    }


    long get_timestamp_us() {
        struct timespec time_stamp;

        clock_gettime(CLOCK_REALTIME, &time_stamp);

        long timestamp_us = time_stamp.tv_sec * 1000000 + time_stamp.tv_nsec / 1000;
        return timestamp_us;
    }

/*
 * return payload size(B) or -1
 *
 * format:
 *  1B 1KB 1MB 1GB
 * */
    long parse_payload_size(const char *input) {
        long payload_size = 0;
        const char *ptr = input;
        char prefix = '&';
        bool end_flag = false;

        // remove the space at the beginning
        for (; *ptr; ptr++) {
            if (*ptr == ' ')
                continue;
            break;
        }

        for (; char ch = *ptr; ptr++) {
            if (end_flag) break;

            switch (ch) {
                case 'B':
                    switch (prefix) {
                        case '&':
                            payload_size *= 1;
                            break;
                        case 'K':
                            payload_size *= 1024;
                            break;
                        case 'M':
                            payload_size *= (1024 * 1024);
                            break;
                        case 'G':
                            payload_size *= (1024 * 1024 * 1024);
                            break;
                    }
                    end_flag = true;
                    break;
                case 'K':
                case 'M':
                case 'G':
                    prefix = ch;
                    break;
                default:
                    if (ch <= '9' && ch >= '0') {
                        payload_size = payload_size * 10 + (ch - '0');
                    } else {
                        return -1;
                    }
                    break;
            }
        }

        for (; *ptr; ptr++) {
            if (*ptr == ' ')
                continue;
            return -1;
        }

        if (!end_flag) {
            return -1;
        }

        return payload_size;
    }
}

#endif //SPHEROMONE_UTILS_H
