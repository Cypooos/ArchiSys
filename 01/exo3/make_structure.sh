#!/bin/bash

if [[ $# -ne 1 ]]; then
    echo -e "\e[31mPlease enter an argument\e[0m" >&2
    exit 2 # 2 for invalid argument, it's what's used by `ls` for example
else 
    directories="$1/src/include $1/src/lib $1/bin $1/test/script $1/test/results"
    files_to_make="$1/src/main.c $1/test/script/template.txt $1/test/results/template.txt"
    mkdir -p $directories
    touch $files_to_make
    find $(pwd)/$1 | grep .txt # pas besoin de définir une fonction récursive bash...
fi

# pour montrer que je sais faire des fonctions bash, voici mon code récursif n'utilisant que ls:
f() {
    local txt=($(ls -lx1p $1 2>/dev/null | grep .txt ));
    for x in ${txt[@]}; do
        echo "$1/$x"
    done
    local dirs=($(ls -lx1p $1 2>/dev/null | grep '/$' ));
    for x in ${dirs[@]}; do
        f "$1/$x"
    done
} 
# évaluer f "$(pwd)/$1"; pour avoir le résultat attendu