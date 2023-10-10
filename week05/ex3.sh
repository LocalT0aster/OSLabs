#!/bin/bash
gcc ex3.c -o ex3.exe -O2 -lpthread -lm -Wextra -Wpedantic
echo "Timing results:" > ex3_res.txt
for m in 1 2 4 10 100
do
    echo "Testing for m=$m"
    { time ./ex3.exe 10000000 $m; } 2>> ex3_res.txt
done
rm ex3.exe
