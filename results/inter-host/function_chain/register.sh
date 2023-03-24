#!/bin/bash

ip=$(ifconfig -a | grep inet | grep -v 127.0.0.1 | grep -v inet6 | awk '{print $2}' | tr -d "addr:" | grep "192.168.1.129")

build_dir="/tmp"

if [[ $ip == "192.168.1.129" ]]; then
  build_dir=${build_dir}"/SPheromone129"
else
  build_dir=${build_dir}"/SPheromone126"
fi

export MANAGER_IP="127.0.0.1"

${build_dir}/benchmarks/00/exp00_register