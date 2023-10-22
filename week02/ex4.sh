#!/bin/bash
gcc -O3 ex4.c -o ex4.out
# Check if an argument is provided and run the compiled program accordingly
if [ "$#" -eq 1 ]; then
    ./ex4 "$1"
else
    ./ex4.out
fi
rm ex4.out
