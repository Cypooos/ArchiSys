#!/bin/bash

if [[ $# -ne 1 ]]; then
    echo -e "\e[31mPlease enter a single file as the argument\e[0m" >&2
    exit 2
else 
    cat $1 2> /dev/null | grep 2
    if [[ "$?" -ne 0 ]]; then
        echo -e "\e[31mUnknow file?\e[0m" 
        exit 2
    fi
fi