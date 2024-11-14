#!/bin/bash
sudo mkdir -p /tmp/ex2 && sudo rm -f /tmp/ex2/pagetable || true
# Compile the mmu and pager programs
gcc mmu.c -o mmu.out -Wall
gcc pager.c -o pager.out -Wall

# Check if the compilation was successful
if [ ! -f mmu.out ] || [ ! -f pager.out ]; then
    echo "Compilation failed."
    exit 1
fi

# Run the pager in the background and get its PID
./pager.out 4 2 &
pager_pid=$!

# Run the MMU with the reference string and the pager's PID
./mmu.out 4 R0 R1 W1 R0 R2 W2 R0 R3 W2 $pager_pid

# Wait for the MMU to finish
wait

# Clean up the PID file
rm -f .mmu.pid pager.out mmu.out || true

# Print a message when the script has finished running
echo "Simulation complete."
