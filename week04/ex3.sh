#!/bin/bash

# Compile the C program
gcc ex3.c -o ex3 -lm

# Run the program with n=3 in the background
./ex3 3 &
sleep 16
echo "---- PSTREE OUTPUT FOR n=3 ----"
pstree | grep ex3

# Wait for all processes to finish
sleep 3

# Run the program with n=5 in the background
./ex3 5 &
sleep 26
echo "---- PSTREE OUTPUT FOR n=5 ----"
pstree | grep ex3

# Wait for all processes to finish
sleep 3
rm ex3
