#!/bin/bash

export MANAGER_IP="192.168.1.129"

ip=`ifconfig -a|grep inet|grep -v 127.0.0.1|grep -v inet6|awk '{print $2}'|tr -d "addr:" | grep "192.168.1.129"`

build_dir="/tmp"

if [[ $ip == "192.168.1.129" ]]
then
  build_dir=${build_dir}"/SPheromone129"
else
  build_dir=${build_dir}"/SPheromone126"
fi

${build_dir}/benchmarks/01/exp01_register