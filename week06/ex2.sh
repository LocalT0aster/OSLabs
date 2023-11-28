#!/bin/bash
gcc scheduler.c -o scheduler.out -Wextra -Wpedantic
gcc worker.c -o worker.out -Wextra -Wpedantic
rm scheduler.log
./scheduler.out data.txt
rm scheduler.out
rm worker.out
