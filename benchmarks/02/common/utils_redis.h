#ifndef MEDIA_MICROSERVICES_SRC_UTILS_REDIS_H_
#define MEDIA_MICROSERVICES_SRC_UTILS_REDIS_H_

#include "ClientPool.h"
#include "MCClient.h"
#include "RedisClient.h"

#define REDIS_POOL_MIN_SIZE 128
#define REDIS_POOL_MAX_SIZE 128

namespace media_service {

    ClientPool<RedisClient> *
    init_redis_client_pool(const json &config_json, const std::string &service_name, uint32_t max_size) {
        auto redis = config_json["MediaService"]["redis"];
        if (redis.empty() ||
            !redis.contains("addr") ||
            !redis.contains("port")) {
            return nullptr;
        }
        std::string redis_addr = redis["addr"];
        int redis_port = redis["port"];
        auto redis_client_pool = new ClientPool<RedisClient>(service_name, redis_addr, redis_port, 1, 128, 1000);

        return redis_client_pool;
    }

} // namespace media_service

#endif //MEDIA_MICROSERVICES_SRC_UTILS_MEMCACHED_H_
