#!/bin/bash

if [[ $# -ne 1 ]]; then
    echo -e "\e[31mPlease enter a single file as the argument\e[0m" >&2
    exit 2
else 
    result=($(wc -l $1 2> /dev/null)) 
    if [[ "$?" -ne 0 ]]; then
        echo -e "\e[31mUnknow file?\e[0m" 
        exit 2
    fi
    echo "The file $1 contains ${result[0]} lines."
fi