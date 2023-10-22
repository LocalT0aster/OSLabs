#!/bin/bash
gcc scheduler.c -o scheduler.out -lm -Wextra -Wpedantic
gcc worker.c -o worker.out -lm
rm scheduler.log
./scheduler.out data.txt
rm scheduler.out
rm worker.out
