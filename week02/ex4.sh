#!/bin/bash
gcc -O3 ex4.c -o ex4.exe
# Check if an argument is provided and run the compiled program accordingly
if [ "$#" -eq 1 ]; then
    ./ex4 "$1"
else
    ./ex4.exe
fi
rm ex4.exe
