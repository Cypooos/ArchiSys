#!/bin/bash

if [[ $# -ne 1 ]]; then
    echo -e "\e[31mPlease enter an argument\e[0m" >&2
    exit 2 # 2 for invalid argument, it's what's used by `ls` for example
else 
    values="$1/src/include $1/src/lib $1/bin $1/test/script $1/test/results"
    mkdir -p $values
fi