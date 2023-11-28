#!/bin/bash
gcc scheduler_sjf.c -o scheduler.out -Wextra -Wpedantic
gcc worker.c -o worker.out -Wextra -Wpedantic
./scheduler.out data.txt
rm scheduler.log
rm scheduler.out
rm worker.out
