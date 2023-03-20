#!/bin/bash

log_dir=/tmp/sp01

rm -rf ${log_dir}
mkdir ${log_dir}

export CONFIG_FILE=/home/lgw/SPheromone/config/config.yml

/tmp/SPheromone/manager/manager 1>${log_dir}/manager.log 2>&1 &

/tmp/SPheromone/coordinator/coordinator 1>${log_dir}/coordinator.log 2>&1 &

/tmp/SPheromone/scheduler/scheduler 1>${log_dir}/scheduler.log 2>&1 &

/tmp/SPheromone/executor/executor 1>${log_dir}/executor.log 2>&1 &