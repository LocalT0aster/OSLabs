#!/bin/bash
gcc ex1.c -o ex1.out -Wall
if [ $? -ne 0 ]; then
    echo "Compilation failed!"
    exit 1
fi
./ex1.out
rm ex1.out
