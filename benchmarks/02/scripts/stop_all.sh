#!/bin/bash
build_dir=/tmp/SPheromone129
kill -9 `ps x | grep ${build_dir} | grep -v grep | awk '{print $1}'`