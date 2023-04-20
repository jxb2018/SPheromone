#!/bin/bash

thread_num=80
config_file_129="/home/lgw/SPheromone/results/intra-host/finra/config129.yml"
finra_data_path="/home/lgw/SPheromone/benchmarks/06/data/"

# start executor
for ((i = 0; i < ${thread_num}; i++)); do
   export CONFIG_FILE=${config_file_129};
   export FinraDataPath=${finra_data_path};
   export LUMINE_GATEWAY_ADDR="192.168.1.129";
   export LUMINE_GATEWAY_PORT="8084";
   /tmp/SPheromone129/executor/executor $i 1>/tmp/sp06/executor_${i}.log 2>&1 &
done