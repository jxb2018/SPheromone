#!/bin/bash

ip=$(ifconfig -a | grep inet | grep -v 127.0.0.1 | grep -v inet6 | awk '{print $2}' | tr -d "addr:" | grep "192.168.1.129")

build_dir="/tmp"

if [[ $ip == "192.168.1.129" ]]; then
  build_dir=${build_dir}"/SPheromone129"
else
  build_dir=${build_dir}"/SPheromone126"
fi

export MANAGER_IP="192.168.1.129"

#payloads=("512B" "1KB" "512KB" "1MB")
payloads=("512B" "512KB")
chain_lens=(2 16 32 64 128)

# shellcheck disable=SC2068
for payload in ${payloads[@]}; do
  for chain_len in ${chain_lens[@]}; do
    for i in {1..100}; do
      ${build_dir}/benchmarks/exp00/exp00_invoker ${chain_len}  ${payload}
      sleep 1s
    done
  done
done
