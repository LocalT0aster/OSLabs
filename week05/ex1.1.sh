#!/bin/bash
gcc channel.c -o ex1.1.exe -lm -Wextra -Wpedantic
./ex1.1.exe || true
rm ex1.1.exe
