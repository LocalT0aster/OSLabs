#!/bin/bash
gcc ex3.c -o ex3.out -O1 -lm -Wextra -Wpedantic
if [ $? -ne 0 ]; then
    echo "Compilation failed!"
    exit 1
fi

# Run the program in the background and redirect its output to ex3.log
./ex3.out &> ex3.log &
PID=$!

# Run vmstat in the background for 12 seconds (10 seconds for the program + a buffer of 2 seconds) 
# and redirect its output to vmstat.log
vmstat 1 12

# Wait for our program to finish
wait $PID

# Clean up
rm ex3.out
