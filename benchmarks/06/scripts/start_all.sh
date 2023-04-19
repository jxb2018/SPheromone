#!/bin/bash

log_dir=/tmp/sp06

rm -rf ${log_dir}
mkdir ${log_dir}

build_dir=/tmp/dzl/SPheromone/cmake-build-debug-126-host

export FinraDataPath="/tmp/dzl/SPheromone/benchmarks/06/data/"
export CONFIG_FILE=$(pwd)/../config/config.yml
export MANAGER_IP="127.0.0.1"

${build_dir}/manager/manager 1>${log_dir}/manager.log 2>&1 &

${build_dir}/coordinator/coordinator 1>${log_dir}/coordinator.log 2>&1 &

${build_dir}/scheduler/scheduler 1>${log_dir}/scheduler.log 2>&1 &

thread_num=10

# shellcheck disable=SC2004
for ((i=0; i< ${thread_num};i++));do
  ${build_dir}/executor/executor $i 1>${log_dir}/executor_${i}.log 2>&1 &
done

sleep 1

# register
${build_dir}/benchmarks/exp06/exp06_register 5 1>${log_dir}/register.log 2>&1 &