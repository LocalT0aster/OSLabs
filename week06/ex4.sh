#!/bin/bash
gcc scheduler_rr.c -o scheduler.out -lm -Wextra -Wpedantic
gcc worker.c -o worker.out -lm
./scheduler.out data.txt
rm scheduler.log
rm scheduler.out
rm worker.out
