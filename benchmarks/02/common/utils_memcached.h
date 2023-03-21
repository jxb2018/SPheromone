#ifndef MEDIA_MICROSERVICES_SRC_UTILS_MEMCACHED_H_
#define MEDIA_MICROSERVICES_SRC_UTILS_MEMCACHED_H_

#include "ClientPool.h"
#include "MCClient.h"

#define MEMCACHED_POOL_MIN_SIZE 128
#define MEMCACHED_POOL_MAX_SIZE 128

namespace media_service {

    ClientPool<MCClient> *
    init_memcached_client_pool(const json &config_json, const std::string &service_name, uint32_t max_size) {
        auto memcached = config_json["MediaService"]["memcached"];
        if (memcached.empty() ||
            !memcached.contains("addr") ||
            !memcached.contains("port")) {
            return nullptr;
        }
        std::string mc_addr = memcached["addr"];
        int mc_port = memcached["port"];
        auto mc_client_pool = new ClientPool<MCClient>(service_name, mc_addr, mc_port, 1, 128, 1000);

        return mc_client_pool;
    }

} // namespace media_service

#endif //MEDIA_MICROSERVICES_SRC_UTILS_MEMCACHED_H_
