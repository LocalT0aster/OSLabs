#!/bin/bash
gcc scheduler_mc.c -o scheduler.out -Wall
gcc worker.c -o worker.out -Wall
./scheduler.out data.txt
rm scheduler.log
rm scheduler.out
rm worker.out
