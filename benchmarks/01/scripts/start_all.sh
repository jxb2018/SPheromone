#!/bin/bash

log_dir=/tmp/sp01

rm -rf ${log_dir}
mkdir ${log_dir}

build_dir=/tmp/SPheromone129

export CONFIG_FILE=$(pwd)/../config/config.yml

${build_dir}/manager/manager 1>${log_dir}/manager.log 2>&1 &

${build_dir}/coordinator/coordinator 1>${log_dir}/coordinator.log 2>&1 &

${build_dir}/scheduler/scheduler 1>${log_dir}/scheduler.log 2>&1 &

${build_dir}/executor/executor 1>${log_dir}/executor.log 2>&1 &

sleep 1

# register
${build_dir}/benchmarks/01/exp01_register