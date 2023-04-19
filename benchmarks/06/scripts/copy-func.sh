#!/bin/bash

# 指定需要复制的文件名和复制的份数
fun_dir=/tmp/shm_dzl

file_name="exp06_run_audit.so"
num_copies=5

file_name=${fun_dir}/${file_name}
# 循环复制文件，每个文件名添加后缀_i
for ((i=1;i<=num_copies;i++))
do
    new_file_name="${file_name%.*}"_$i".${file_name##*.}"
    cp "$file_name" "$new_file_name"
done
