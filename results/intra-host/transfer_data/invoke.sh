#!/bin/bash

ip=`ifconfig -a|grep inet|grep -v 127.0.0.1|grep -v inet6|awk '{print $2}'|tr -d "addr:" | grep "192.168.1.129"`

build_dir="/tmp"

if [[ $ip == "192.168.1.129" ]]
then
  build_dir=${build_dir}"/SPheromone129"
else
  build_dir=${build_dir}"/SPheromone126"
fi

export MANAGER_IP="192.168.1.129"

payloads=("256B" "512B" "1KB" "512KB" "1MB" "512MB" "1GB")
#payloads=("256B" "512B" "1KB" "512KB")

# shellcheck disable=SC2068
for payload in ${payloads[@]}; do
  for i in {1..3}; do
    ${build_dir}/benchmarks/01/exp01_invoker "4" ${payload}
    sleep 1s
  done
done