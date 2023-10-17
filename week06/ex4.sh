#!/bin/bash
gcc scheduler_rr.c -o scheduler.exe -lm -Wextra -Wpedantic
gcc worker.c -o worker.exe -lm
./scheduler.exe data.txt
rm scheduler.log
rm scheduler.exe
rm worker.exe
