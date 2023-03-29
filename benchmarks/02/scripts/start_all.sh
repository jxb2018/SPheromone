#!/bin/bash

log_dir=/tmp/sp02

rm -rf ${log_dir}
mkdir ${log_dir}

build_dir=/tmp/SPheromone129

export CONFIG_FILE=$(pwd)/../config/config.yml
export STORE_SERVICE_CONFIG_PATH=$(pwd)/../config/service_config.json
export MANAGER_IP="127.0.0.1"

${build_dir}/manager/manager 1>${log_dir}/manager.log 2>&1 &

${build_dir}/coordinator/coordinator 1>${log_dir}/coordinator.log 2>&1 &

${build_dir}/scheduler/scheduler 1>${log_dir}/scheduler.log 2>&1 &

thread_num=2

# shellcheck disable=SC2004
for ((i = 0; i < ${thread_num}; i++)); do
  ${build_dir}/executor/executor $i 1>${log_dir}/executor_${i}.log 2>&1 &
done

sleep 1

# store review register
${build_dir}/benchmarks/02/exp02_register_test1