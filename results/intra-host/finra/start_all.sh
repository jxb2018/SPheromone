#!/bin/bash

config_file_129="/home/lgw/SPheromone/results/intra-host/finra/config129.yml"
finra_data_path="/home/lgw/SPheromone/benchmarks/06/data/"

# precondition
rm_dir="rm -rf /tmp/sp06; mkdir /tmp/sp06; rm -rf /tmp/shm; mkdir /tmp/shm"

ssh -p 22 lgw@192.168.1.129 "${rm_dir}; cmake --build /tmp/SPheromone129 \
--target manager coordinator scheduler executor \
exp06_marketdata exp06_run_audit exp06_marginbalance exp06_register exp06_invoker -- -j 16"

# start manager coordinator scheduler
ssh -p 22 lgw@192.168.1.129 "export CONFIG_FILE=${config_file_129}; \
/tmp/SPheromone129/manager/manager 1>/tmp/sp06/manager.log 2>&1 & \
/tmp/SPheromone129/coordinator/coordinator 1>/tmp/sp06/coordinator.log 2>&1 & \
/tmp/SPheromone129/scheduler/scheduler 1>/tmp/sp06/scheduler.log 2>&1 &"

thread_num=10
fanout_num=10
# start executor
for ((i = 0; i < ${thread_num}; i++)); do
  ssh -p 22 lgw@192.168.1.129 "export CONFIG_FILE=${config_file_129}; \
   export FinraDataPath=${finra_data_path}; \
   export LUMINE_GATEWAY_ADDR=192.168.1.129; \
   export LUMINE_GATEWAY_PORT=8084; \
   /tmp/SPheromone129/executor/executor $i 1>/tmp/sp06/executor_${i}.log 2>&1 &"
done

# copy library file
for ((i = 1; i <= ${fanout_num}; i++)); do
  ssh -p 22 lgw@192.168.1.129 "cp /tmp/shm/exp06_run_audit.so /tmp/shm/exp06_run_audit_${i}.so"
done

# register
manager_ip="192.168.1.129"

ssh -p 22 lgw@192.168.1.129 "export CONFIG_FILE=${config_file_129}; \
export MANAGER_IP=${manager_ip}; \
 /tmp/SPheromone129/benchmarks/exp06/exp06_register ${fanout_num}"
