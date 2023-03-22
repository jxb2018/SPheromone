#!/bin/bash

ip=`ifconfig -a|grep inet|grep -v 127.0.0.1|grep -v inet6|awk '{print $2}'|tr -d "addr:" | grep "192.168.1.129"`

build_dir="/tmp"
config_file="/home/lgw/SPheromone/results/intra-host/transfer_data"

if [[ $ip == "192.168.1.129" ]]
then
  build_dir=${build_dir}"/SPheromone129"
  config_file=${config_file}"/config129.yml"
else
  build_dir=${build_dir}"/SPheromone126"
  config_file=${config_file}"/config126.yml"
fi


log_dir=/tmp/sp01

rm -rf ${log_dir}
mkdir ${log_dir}

export CONFIG_FILE=${config_file}
export MANAGER_IP="192.168.1.129"

${build_dir}/manager/manager 1>${log_dir}/manager.log 2>&1 &

${build_dir}/coordinator/coordinator 1>${log_dir}/coordinator.log 2>&1 &

${build_dir}/scheduler/scheduler 1>${log_dir}/scheduler.log 2>&1 &

thread_num=4

# shellcheck disable=SC2004
for ((i=0; i< ${thread_num};i++));do
  ${build_dir}/executor/executor $i 1>${log_dir}/executor_${i}.log 2>&1 &
done

sleep 1

${build_dir}/benchmarks/01/exp01_register
