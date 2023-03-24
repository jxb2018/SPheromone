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
#include "fmt/format.h"

template<class T>
using vector = std::vector<T>;

#define BUCKET_REQ_PORT 12070
#define BUCKET_OP_PORT  7800
#define TRIGGER_REQ_PORT 12170
#define TRIGGER_OP_PORT 7900

#define TUPLE std::tuple<std::vector<std::string>, std::vector<std::string>, DependencyType>

class OperationRequestThread {
public:
    OperationRequestThread(std::string ip_addr, int thread_id) :
            ip_addr_(ip_addr), thread_id_(thread_id) {};

    std::string bucket_req_connect_addr() {
        return "tcp://" + ip_addr_ + ":" + std::to_string(BUCKET_REQ_PORT + thread_id_);
    }

    std::string bucket_req_bind_address() {
        return "tcp://*:" + std::to_string(BUCKET_REQ_PORT + thread_id_);
    }

    std::string trigger_req_connect_addr() {
        return "tcp://" + ip_addr_ + ":" + std::to_string(TRIGGER_REQ_PORT + thread_id_);
    }

    std::string trigger_req_bind_address() {
        return "tcp://*:" + std::to_string(TRIGGER_REQ_PORT + thread_id_);
    }

private:
    std::string ip_addr_;
    int thread_id_;
};


class PheromoneClient {
public:
    explicit PheromoneClient(std::string manager_ip, int thread_id = 0);

    ~PheromoneClient() {
        manager_socket_->close();
        delete manager_socket_;

        bucket_op_puller_->close();
        delete bucket_op_puller_;

        trigger_op_puller_->close();
        delete trigger_op_puller_;

        delete pushers_;
        delete context_;
        delete kZmqUtil_;
        delete ort_;
    }

    void register_app(std::string app_name, std::vector<std::string> funcs,
                      std::vector<TUPLE> dependency);

    void create_bucket(std::string app_name, std::string bucket_name);

    void
    add_trigger(std::string app_name, std::string bucket_name, std::string trigger_name, PrimitiveType primitive_type,
                std::map<std::string, std::string> primitive, int trigger_option);

    void call_app(std::string app_name, std::string func_name, std::vector<std::string>);

private:
    std::string manager_ip_;

    int thread_id_;

    int req_id;

    OperationRequestThread *ort_;

    ZmqUtilInterface *kZmqUtil_;

    map<string, pair<string, unsigned>> app_coord_map_;

    zmq::context_t *context_;

    SocketCache *pushers_;

    zmq::socket_t *manager_socket_;

    zmq::socket_t *bucket_op_puller_;

    zmq::socket_t *trigger_op_puller_;

    std::pair<std::string, unsigned> get_coord(std::string app_name);

    std::string get_request_id() {
        return std::to_string(req_id++);
    }

};

PheromoneClient::PheromoneClient(std::string manager_ip, int thread_id) {
    manager_ip_ = std::move(manager_ip);
    thread_id_ = thread_id;

    req_id = 0;
    ort_ = new OperationRequestThread(manager_ip_, thread_id);

    kZmqUtil_ = new ZmqUtil();
    context_ = new zmq::context_t(1);
    pushers_ = new SocketCache(context_, ZMQ_PUSH);
    manager_socket_ = new zmq::socket_t(*context_, ZMQ_REQ);
    bucket_op_puller_ = new zmq::socket_t(*context_, ZMQ_PULL);
    trigger_op_puller_ = new zmq::socket_t(*context_, ZMQ_PULL);

    manager_socket_->connect("tcp://" + manager_ip_ + ":" + std::to_string(6002));
    bucket_op_puller_->bind(ort_->bucket_req_bind_address());
    trigger_op_puller_->bind(ort_->trigger_req_bind_address());
}

void PheromoneClient::register_app(std::string app_name, std::vector<std::string> funcs,
                                   std::vector<TUPLE> dependency) {
    AppRegistration msg;

    auto coord_thread = get_coord(app_name);

    for (auto &func: funcs)
        msg.add_functions(func);

    for (auto &d : dependency){
        auto dep = msg.add_dependencies();
        for (auto &src : std::get<0>(d)){
            dep->add_src_functions(src);
        }

        for (auto &tgt : std::get<1>(d)){
            dep->add_tgt_functions(tgt);
        }
        dep->set_type(std::get<2>(d));

        // TODO: len(d) >= 4
    }

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

void PheromoneClient::call_app(std::string app_name, std::string func_name, std::vector<std::string> args) {
    auto coord_thread = get_coord(app_name);

    FunctionCall call;
    call.set_app_name(app_name);
    auto req = call.add_requests();
    req->set_name(func_name);
    for (auto &argument: args) {
        auto arg = req->add_arguments();
        arg->set_body(argument);
        arg->set_arg_flag(0);
    }
    std::string serialized;
    call.SerializeToString(&serialized);

    auto socket = &(pushers_->At("tcp://" + coord_thread.first + ":" +
                                 std::to_string(coord_thread.second + 5050)));

    kZmqUtil_->send_string(serialized, socket);
}

void PheromoneClient::add_trigger(std::string app_name, std::string bucket_name, std::string trigger_name,
                                  PrimitiveType primitive_type, std::map<std::string, std::string> primitive,
                                  int trigger_option) {
    auto coord_thread = get_coord(app_name);

    // step1: prepare
    TriggerOperationRequest req;

    req.set_operation_type(ADD_TRIGGER);
    req.set_request_id(get_request_id());
    req.set_response_address(ort_->trigger_req_connect_addr());
    req.set_app_name(app_name);
    req.set_bucket_name(bucket_name);
    req.set_trigger_name(trigger_name);
    req.set_primitive_type(primitive_type);
    req.set_trigger_option(trigger_option);

    std::string prm_serialized;
    if (primitive_type == IMMEDIATE) {
        auto prm = ImmediatePrimitive();
        prm.set_function(primitive["function"]);
        prm.SerializeToString(&prm_serialized);
    } else if (primitive_type == BY_SET) {
        auto prm = BySetPrimitive();
        prm.set_function(primitive["function"]);

        auto key_names = primitive["key_name"];
        char *remain;
        auto ptr = strtok_r(const_cast<char *>(key_names.c_str()), " ", &remain);
        while (ptr != nullptr) {
            prm.add_key_set(ptr);
            ptr = strtok_r(nullptr, " ", &remain);
        }
        prm.SerializeToString(&prm_serialized);
    }
    req.set_primitive(prm_serialized);

    // step2: send
    std::string serialized;
    req.SerializeToString(&serialized);

    auto socket = &(pushers_->At("tcp://" + coord_thread.first + ":" +
                                 std::to_string(coord_thread.second + TRIGGER_OP_PORT)));
    kZmqUtil_->send_string(serialized, socket);

    // step3: receive
    auto serialized_resp = kZmqUtil_->recv_string(trigger_op_puller_);
    TriggerOperationResponse resp;
    resp.ParseFromString(serialized_resp);

    if (resp.error() == 0) {
        std::cout << fmt::format("Successfully creating trigger {}", trigger_name) << std::endl;
    } else {
        std::cerr << fmt::format("Failed to create trigger {}", trigger_name) << std::endl;
    }
}

void PheromoneClient::create_bucket(std::string app_name, std::string bucket_name) {
    auto coord_thread = get_coord(app_name);

    // step1: prepare
    auto req = BucketOperationRequest();
    req.set_operation_type(CREATE_BUCKET);
    req.set_request_id(get_request_id());
    req.set_response_address(ort_->bucket_req_connect_addr());
    req.set_app_name(app_name);
    req.set_bucket_name(bucket_name);

    // step2: send
    std::string serialized;
    req.SerializeToString(&serialized);

    auto socket = &(pushers_->At("tcp://" + coord_thread.first + ":" +
                                 std::to_string(coord_thread.second + BUCKET_OP_PORT)));
    kZmqUtil_->send_string(serialized, socket);

    // step3: recv
    auto serialized_resp = kZmqUtil_->recv_string(bucket_op_puller_);
    BucketOperationResponse resp;
    resp.ParseFromString(serialized_resp);

    if (resp.error() == 0) {
        std::cout << fmt::format("Successfully creating bucket {}", bucket_name) << std::endl;
    } else {
        std::cerr << fmt::format("Failed to create bucket {}", bucket_name) << std::endl;
    }

}

#endif //SPHEROMONE_PHEROMONE_CLIENT_H
