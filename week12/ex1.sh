#!/bin/bash
gcc ex1.c -o ex1.out -O2 -lm -Wextra -Wpedantic
if [ $? -ne 0 ]; then
    echo "Compilation failed!"
    exit 1
fi
./ex1.out
rm ex1.out
