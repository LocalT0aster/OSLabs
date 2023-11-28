#!/bin/bash
gcc ex4.c -o ex4.out -lpthread -Wextra -Wpedantic
echo "Timing results:" > ex4_res.txt
for m in 1 2 4 10 100
do
    echo "Testing for m=$m"
    { time ./ex4.out 10000000 $m; } 2>> ex4_res.txt
done
rm ex4.out
