#!/bin/bash
gcc scheduler.c -o scheduler.out -Wall
gcc worker.c -o worker.out -Wall
rm scheduler.log
./scheduler.out data.txt
rm scheduler.out
rm worker.out
