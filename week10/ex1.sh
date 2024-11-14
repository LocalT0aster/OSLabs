#!/bin/bash
gcc monitor.c -o monitor.out -Wall
gcc ex1.c -o ex1.out -Wall
if [ $? -ne 0 ]; then
    echo "Compilation failed!"
    exit 1
fi

# Run monitor.c in the background on the specified directory
./monitor.out "./watchdir/" &

# Run ex1.c
./ex1.out

# Kill monitor
kill $1

rm ex1.out monitor.out

