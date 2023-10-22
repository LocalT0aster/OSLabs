#!/bin/bash
gcc allocator.c -o allocator.out -lm -Wextra -Wpedantic
./allocator.out
rm allocator.out
