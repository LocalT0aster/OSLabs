#!/bin/bash
gcc ex1.c -o ex1.out -O3 -Wall -Wno-format-security
if [ $? -ne 0 ]; then
    echo "Compilation failed!"
    exit 1
fi
sudo ./ex1.out
rm ex1.out
