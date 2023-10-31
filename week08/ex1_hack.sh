#!/bin/bash
# Compilation
gcc ex1.c -o ex1.out -lm -O0 -Wextra -Wpedantic
sudo echo "Access Granted"


sudo ./ex1.out &
sleep 2
echo "Executing search..."
# Get the pid from /tmp/ex1.pid
PID=$(cat /tmp/ex1.pid)
echo "The target PID is $PID"

FOUND=false
# Read through the memory mappings to identify potential ranges
while read -r line; do
    # Extract start and end addresses of the range
    START_ADDR=$(echo $line | awk '{print $1}' | cut -d'-' -f1)
    END_ADDR=$(echo $line | awk '{print $1}' | cut -d'-' -f2)

    # Extract the content from /proc/$PID/mem for the specific range
    MEMORY_CONTENT=$(sudo dd if=/proc/$PID/mem bs=1 count=$((0x$END_ADDR - 0x$START_ADDR)) skip=$((0x$START_ADDR)) 2>/dev/null)
    CONTENT=$(echo "$MEMORY_CONTENT" | grep -a -o -P "pass:.{9}")

    # If the content has our password pattern
    if [[ $CONTENT == pass:* ]]; then
        # Find the offset of the password in the extracted content
        OFFSET=$(echo "$MEMORY_CONTENT" | grep -a -o -b -P "pass:.{9}" | cut -d: -f1)
        
        # Calculate the exact address
        EXACT_ADDRESS=$(printf "%x\n" $(($((0x$START_ADDR)) + $OFFSET)))

        echo "Found password: $CONTENT at exact address 0x$EXACT_ADDRESS"
        FOUND=true
        break
    fi
done < <(sudo grep rw-p /proc/$PID/maps)

if [ "$FOUND" = false ]; then
    echo "Password not found."
fi

# Send SIGKILL to the program
sudo kill -9 $PID
rm ex1.out
