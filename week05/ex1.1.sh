#!/bin/bash
gcc channel.c -o ex1.1.out -lm -Wextra -Wpedantic
./ex1.1.out || true
rm ex1.1.out
