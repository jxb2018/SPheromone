#ifndef MEDIA_MICROSERVICES_REDISCLIENT_H
#define MEDIA_MICROSERVICES_REDISCLIENT_H

#include <memory>
#include <string>

#include "MediaServiceTypes.h"
#include "logger.h"
#include "GenericClient.h"
#include "FaasWorker.h"
#include "hiredis.h"

namespace media_service {

    class RedisClient : public GenericClient {
    public:
        RedisClient(const std::string &addr, int port, const std::string &http_path, FaasWorker *faas_worker,
                    uint16_t client_id);

        RedisClient(const RedisClient &) = delete;

        RedisClient &operator=(const RedisClient &) = delete;

        RedisClient(RedisClient &&) = default;

        RedisClient &operator=(RedisClient &&) = default;

        ~RedisClient() override;

        class RedisReplyWrapper {
        public:
            explicit RedisReplyWrapper(redisReply *reply, bool need_free = true)
                    : _reply(reply), _need_free(need_free) {}

            ~RedisReplyWrapper() { if (_need_free) freeReplyObject(_reply); }

            std::string as_string() const {
                if (_reply->type != REDIS_REPLY_STRING) {
                    fprintf(stderr, "Failed to retrieve message from Redis");
                    return nullptr;
                }
                return std::string(_reply->str, _reply->len);
            }

            long long as_integer() const {
                if (_reply->type != REDIS_REPLY_INTEGER) {
                    fprintf(stderr, "Failed to retrieve message from Redis");
                    return -1;
                }
                return _reply->integer;
            }

            std::vector<std::unique_ptr<RedisReplyWrapper>> as_array() {
                if (_reply->type != REDIS_REPLY_ARRAY) {
                    fprintf(stderr, "Failed to retrieve message from Redis");
                    throw nullptr;
                }
                std::vector<std::unique_ptr<RedisReplyWrapper>> ret;
                for (size_t i = 0; i < _reply->elements; i++) {
                    ret.emplace_back(new RedisReplyWrapper(_reply->element[i], false));
                }
                return ret;
            }

            bool ok() const {
                if (_reply->type == REDIS_REPLY_ERROR) return false;
                if (_reply->type == REDIS_REPLY_STATUS
                    && strncmp(_reply->str, "OK", 2) != 0) {
                    return false;
                }
                return true;
            }

            void check_ok() const {
                if (!ok()) {
                    fprintf(stderr, "Failed to retrieve message from Redis");
                    return;
                }
            }

        private:
            redisReply *_reply;
            bool _need_free;
        };

        class RedisContextWrapper {
        public:
            explicit RedisContextWrapper(redisContext *context) : _context(context) {}

            void AppendCommand(const char *format, ...) {
                va_list ap;
                va_start(ap, format);
                int ret = redisvAppendCommand(_context, format, ap);
                va_end(ap);
                if (ret != REDIS_OK) {
                    fprintf(stderr, "Failed to retrieve message from Redis");
                    return;
                }
            }

            std::unique_ptr<RedisReplyWrapper> GetReply() {
                redisReply *reply = nullptr;
                if (redisGetReply(_context, (void **) &reply) != REDIS_OK) {
                    if (reply != nullptr) freeReplyObject(reply);
                    fprintf(stderr, "Failed to retrieve message from Redis");
                    return nullptr;
                }
                return std::make_unique<RedisReplyWrapper>(reply);
            }

        private:
            redisContext *_context;
        };

        RedisContextWrapper GetClient() const;

        void Connect() override;

        void Disconnect() override;

        void KeepAlive() override;

        void KeepAlive(int timeout_ms) override;

        bool IsConnected() override;

    private:
        redisContext *_client;
    };

    RedisClient::RedisClient(const std::string &addr, int port,
                             const std::string &http_path, FaasWorker *faas_worker, uint16_t client_id) {
        _addr = addr;
        _port = port;
        _client_id = client_id;
        _client = nullptr;
    }

    RedisClient::~RedisClient() {
        Disconnect();
        delete _client;
    }

    RedisClient::RedisContextWrapper RedisClient::GetClient() const {
        return RedisContextWrapper(_client);
    }

    void RedisClient::Connect() {
        if (!IsConnected()) {
            _client = redisConnect(_addr.c_str(), _port);
            if (_client == nullptr || _client->err) {
                LOG(error) << "Failed to connect " << _addr << ":" << _port;
                fprintf(stderr, "Failed to connect");
                return;
            }
        }
    }

    void RedisClient::Disconnect() {
        if (IsConnected()) {
            redisFree(_client);
            _client = nullptr;
        }
    }

    bool RedisClient::IsConnected() {
        return _client != nullptr;
    }

    void RedisClient::KeepAlive() {}

    void RedisClient::KeepAlive(int timeout_ms) {}

} // mediua_service

#endif //MEDIA_MICROSERVICES_REDISCLIENT_H
