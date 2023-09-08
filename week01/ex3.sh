#!/bin/bash
declare -A commands=(
    ["/"]="root"
    ["/home/${USER}"]="home"
)

for path in "${!commands[@]}"; do
    mkdir -p "${commands[$path]%/*}"
    date && sleep 3
    ls -latr "${path}" > "${commands[$path]}/${commands[$path]}.txt"
    date && sleep 3
done
