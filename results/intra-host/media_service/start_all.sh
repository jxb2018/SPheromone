#!/bin/bash

config_file_129="/home/lgw/SPheromone/results/intra-host/media_service/config129.yml"
service_config_file="/home/lgw/SPheromone/benchmarks/02/config/service_config.json"

# precondition
rm_dir="rm -rf /tmp/sp02; mkdir /tmp/sp02"

# start manager coordinator scheduler
ssh -p 22 lgw@192.168.1.129 "${rm_dir}; export CONFIG_FILE=${config_file_129}; \
export STORE_SERVICE_CONFIG_PATH=${service_config_file}; \
/tmp/SPheromone129/manager/manager 1>/tmp/sp02/manager.log 2>&1 & \
/tmp/SPheromone129/coordinator/coordinator 1>/tmp/sp02/coordinator.log 2>&1 & \
/tmp/SPheromone129/scheduler/scheduler 1>/tmp/sp02/scheduler.log 2>&1 &"

thread_num=80
# start executor
ssh -p 22 lgw@192.168.1.129 "for ((i = 0; i < ${thread_num}; i++)); do export CONFIG_FILE=${config_file_129}; \
   export STORE_SERVICE_CONFIG_PATH=${service_config_file}; \
   export LUMINE_GATEWAY_ADDR=192.168.1.129; \
   export LUMINE_GATEWAY_PORT=8084; \
   /tmp/SPheromone129/executor/executor \$i 1>/tmp/sp02/executor_\${i}.log 2>&1 & done"

manager_ip="192.168.1.129"
# register
ssh -p 22 lgw@192.168.1.129 "export CONFIG_FILE=${config_file_129}; \
export MANAGER_IP=${manager_ip}; \
 /tmp/SPheromone129/benchmarks/exp02/exp02_register_test1"
