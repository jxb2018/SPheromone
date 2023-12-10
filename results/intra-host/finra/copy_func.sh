#!/bin/bash

fun_dir=/tmp/shm

file_name="exp06_run_audit.so"
num_copies=80

file_name=${fun_dir}/${file_name}
#for ((i=1;i<=num_copies;i++))
#do
#    new_file_name="${file_name%.*}"_$i".${file_name##*.}"
#    cp "$file_name" "$new_file_name"
#done


ssh -p 22 lgw@192.168.1.129 "for ((i = 1; i <= ${num_copies}; i++)); do \
    new_file_name=${file_name%.*}_\${i}.so
    cp ${file_name} \${new_file_name}; \
done"
