#!/bin/bash
gcc agent.c -o agent.exe -lm -Wextra -Wpedantic
gcc controller.c -o controller.exe -lm -Wextra -Wpedantic

./agent.exe &
sleep 1
./controller.exe

rm agent.exe
rm controller.exe
rm /var/run/agent.pid
