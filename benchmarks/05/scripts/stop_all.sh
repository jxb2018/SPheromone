#!/bin/bash
build_dir=SPheromone
#build_dir=/tmp/lumine-benchmarks
kill -9 `ps x | grep ${build_dir} | grep -v grep | awk '{print $1}'`