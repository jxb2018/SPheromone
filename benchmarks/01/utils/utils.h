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

namespace exp01 {

    struct Head {
        long chain_length;
        long payload_size;
        long send_time;
    };

    static_assert(sizeof(Head) == 24);

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
