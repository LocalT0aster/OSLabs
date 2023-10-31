#!/bin/bash
gcc ex2.c -o ex2.out -lm -Wextra -Wpedantic
if [ $? -ne 0 ]; then
    echo "Compilation failed!"
    exit 1
fi
./ex2.out
rm ex2.out
