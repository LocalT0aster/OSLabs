#!/bin/bash
gcc ex2.c -o ex2.out -O2 -lpthread -Wall
rm text.txt || true
./ex2.out
rm ex2.out
