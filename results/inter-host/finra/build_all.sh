#!/bin/bash

fanout_num=80

# precondition
ssh -p 22 lgw@192.168.1.129 "cmake --build /tmp/SPheromone129 \
--target manager coordinator scheduler executor \
exp06_marketdata exp06_run_audit exp06_marginbalance exp06_register exp06_invoker -- -j 16"

ssh -p 22 lgw@192.168.1.126 "cmake --build /tmp/SPheromone126 \
--target manager coordinator scheduler executor \
exp06_marketdata exp06_run_audit exp06_marginbalance exp06_register exp06_invoker -- -j 16"

# copy library file
ssh -p 22 lgw@192.168.1.129 "for ((i = 1; i <= ${fanout_num}; i++)); do cp /tmp/shm/exp06_run_audit.so /tmp/shm/exp06_run_audit_\${i}.so; done"
ssh -p 22 lgw@192.168.1.126 "for ((i = 1; i <= ${fanout_num}; i++)); do cp /tmp/shm/exp06_run_audit.so /tmp/shm/exp06_run_audit_\${i}.so; done"