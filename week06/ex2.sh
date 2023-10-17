#!/bin/bash
gcc scheduler.c -o scheduler.exe -lm -Wextra -Wpedantic
gcc worker.c -o worker.exe -lm
./scheduler.exe data.txt
rm scheduler.exe
rm worker.exe
