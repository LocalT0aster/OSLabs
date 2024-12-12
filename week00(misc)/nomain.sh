#!/bin/bash
gcc -nostartfiles -Wl,-eentry -o nomain.out nomain.c
./nomain.out
rm nomain.out
