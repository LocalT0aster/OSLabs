#!/bin/bash
if [ "$#" -ne 1 ]; then
    echo "Usage: ./ex2.sh <number of threads>"
    exit 1
fi
gcc ex2.c -o ex2.exe -pthread -lm -Wextra -Wpedantic
./ex2.exe $1
rm ex2.exe
