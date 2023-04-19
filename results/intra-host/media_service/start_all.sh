#!/bin/bash

config_file_129="/home/lgw/SPheromone/results/intra-host/media_service/config129.yml"
service_config_file="/home/lgw/SPheromone/benchmarks/02/config/service_config.json"

# precondition
rm_dir="rm -rf /tmp/sp02; mkdir /tmp/sp02"

ssh -p 22 lgw@192.168.1.192 "${rm_dir}; cmake --build /tmp/SPheromone129 \
--target manager coordinator scheduler executor \
exp02_store_review_4 exp02_upload_text_3 exp02_upload_uniqueid_3 exp02_upload_rating_3 exp02_upload_movieid_3 \
exp02_upload_rating_2 exp02_upload_uniqueid_1 exp02_upload_text_1 exp02_upload_movieid_1 exp02_register_movieid_1 \
exp02_web_server_0 exp02_register_test1 exp02_invoker_test1 -- -j 16"

# start manager coordinator scheduler
ssh -p 22 lgw@192.168.1.192 "export CONFIG_FILE=${config_file_129}; \
export STORE_SERVICE_CONFIG_PATH=${service_config_file}; \
/tmp/SPheromone129/manager/manager 1>/tmp/sp02/manager.log 2>&1 & \
/tmp/SPheromone129/coordinator/coordinator 1>/tmp/sp02/coordinator.log 2>&1 & \
/tmp/SPheromone129/scheduler/scheduler 1>/tmp/sp02/scheduler.log 2>&1 &"

thread_num=24
# start executor
for ((i = 0; i < ${thread_num}; i++)); do
  ssh -p 22 lgw@192.168.1.192 "export CONFIG_FILE=${config_file_129}; \
   export STORE_SERVICE_CONFIG_PATH=${service_config_file}; \
   export LUMINE_GATEWAY_ADDR=192.168.1.129; \
   export LUMINE_GATEWAY_PORT=8084; \
   /tmp/SPheromone129/executor/executor $i 1>/tmp/sp02/executor_${i}.log 2>&1 &"
done

manager_ip="192.168.1.129"
# register
ssh -p 22 lgw@192.168.1.192 "export CONFIG_FILE=${config_file_129}; \
export MANAGER_IP=${manager_ip}; \
 /tmp/SPheromone129/benchmarks/exp02/exp02_register_test1"
