#!/bin/bash


ssh -p 22 lgw@192.168.1.129 "kill -9 \$(ps x | grep '/tmp/SPheromone129' | grep -v grep | awk '{print \$1}')"
