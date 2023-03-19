//
// Created by tank on 3/19/23.
//

#ifndef SPHEROMONE_PHEROMONE_CLIENT_H
#define SPHEROMONE_PHEROMONE_CLIENT_H

#include <string>
#include <signal.h>
#include <iostream>
#include <string>
#include <cstring>
#include <thread>
#include <chrono>
#include <cstddef>

#include "operation.pb.h"
#include "types.hpp"
#include "common.hpp"
#include "requests.hpp"
#include "kvs_threads.hpp"
#include "trigger.hpp"
#include "yaml-cpp/yaml.h"
#include "zmq/zmq_util.hpp"

template<class T>
using vector = std::vector<T>;

class PheromoneClient {
public:
    explicit PheromoneClient(std::string manager_ip);

    ~PheromoneClient() {
        manager_socket_->close();
        delete manager_socket_;
        delete pushers_;
        delete context_;
        delete kZmqUtil_;
    }

    void register_app(std::string app_name, std::vector<std::string> funcs);

    void call_app(std::string app_name, std::string func_name, std::string args);

private:
    std::string manager_ip_;

    ZmqUtilInterface *kZmqUtil_;

    map<string, pair<string, unsigned>> app_coord_map_;

    zmq::context_t *context_;

    SocketCache *pushers_;

    zmq::socket_t *manager_socket_;

    std::pair<std::string, unsigned> get_coord(std::string app_name);
};

PheromoneClient::PheromoneClient(std::string manager_ip) {
    manager_ip_ = std::move(manager_ip);

    kZmqUtil_ = new ZmqUtil();
    context_ = new zmq::context_t(1);
    pushers_ = new SocketCache(context_, ZMQ_PUSH);
    manager_socket_ = new zmq::socket_t(*context_, ZMQ_REQ);

    manager_socket_->connect("tcp://" + manager_ip_ + ":" + std::to_string(6002));
}

void PheromoneClient::register_app(std::string app_name, std::vector<std::string> funcs) {
    AppRegistration msg;

    auto coord_thread = get_coord(app_name);

    for (auto &func: funcs)
        msg.add_functions(func);

    std::string serialized;
    msg.SerializeToString(&serialized);


    auto socket = &(pushers_->At("tcp://" + coord_thread.first + ":" +
                                 std::to_string(coord_thread.second + 5020)));

    kZmqUtil_->send_string(serialized, socket);

    std::cout << fmt::format("app {} registered", app_name) << std::endl;
}

std::pair<std::string, unsigned> PheromoneClient::get_coord(string app_name) {
    if (app_coord_map_.find(app_name) == app_coord_map_.end()) {
        CoordQuery query;
        query.set_application(app_name);

        std::string serialized;
        query.SerializeToString(&serialized);
        kZmqUtil_->send_string(serialized, manager_socket_);

        std::string serialized_resp = kZmqUtil_->recv_string(manager_socket_);
        CoordResponse resp;
        resp.ParseFromString(serialized_resp);
        app_coord_map_[app_name] = std::make_pair(resp.coord_ip(), resp.thread_id());
    }
    return app_coord_map_[app_name];
}

void PheromoneClient::call_app(std::string app_name, std::string func_name, std::string args) {
    auto coord_thread = get_coord(app_name);

    FunctionCall call;
    call.set_app_name(app_name);
    auto req = call.add_requests();
    req->set_name(func_name);
    auto arg = req->add_arguments();
    arg->set_body(args);
    arg->set_arg_flag(0);

    std::string serialized;
    call.SerializeToString(&serialized);

    auto socket = &(pushers_->At("tcp://" + coord_thread.first + ":" +
                                 std::to_string(coord_thread.second + 5050)));

    kZmqUtil_->send_string(serialized, socket);
}

#endif //SPHEROMONE_PHEROMONE_CLIENT_H
