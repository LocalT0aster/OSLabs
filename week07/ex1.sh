#!/bin/bash
gcc allocator.c -o allocator.out -Wextra -Wpedantic
./allocator.out
rm allocator.out
