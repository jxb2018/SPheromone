#!/bin/bash

ssh -p 22 lgw@192.168.1.129 "export MANAGER_IP=127.0.0.1; /tmp/SPheromone129/benchmarks/exp06/exp06_invoker 10 1000001"