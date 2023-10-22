#!/bin/bash
gcc publisher.c -o publisher.out -lm -Wextra -Wpedantic
gcc subscriber.c -o subscriber.out -lm -Wextra -Wpedantic

if [ "$#" -ne 1 ]; then
    echo "Usage: $0 <number of subscribers>"
    exit 1
fi

n=$1

x-terminal-emulator -e "./publisher.out $n" &

sleep 1  # wait for a while to ensure publisher sets up pipes

for ((i=1; i<=$n; i++))
do
    x-terminal-emulator -e "./subscriber.out $i" &
done

wait
rm publisher.out
rm subscriber.out
