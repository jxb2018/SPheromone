#!/bin/bash
build_dir=/tmp/SPheromone
#build_dir=/tmp/lumine-benchmarks
kill `ps x | grep ${build_dir} | grep -v grep | awk '{print $1}'`