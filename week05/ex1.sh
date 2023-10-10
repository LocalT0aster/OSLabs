#!/bin/bash
gcc publisher.c -o publisher.exe -lm -Wextra -Wpedantic
gcc subscriber.c -o subscriber.exe -lm -Wextra -Wpedantic

if [ "$#" -ne 1 ]; then
    echo "Usage: $0 <number of subscribers>"
    exit 1
fi

n=$1

x-terminal-emulator -e "./publisher.exe $n" &

sleep 1  # wait for a while to ensure publisher sets up pipes

for ((i=1; i<=$n; i++))
do
   x-terminal-emulator -e "./subscriber.exe $i" &
done

wait
rm publisher.exe
rm subscriber.exe
