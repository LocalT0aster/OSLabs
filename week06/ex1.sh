#!/bin/bash
gcc agent.c -o agent.exe -lm -Wextra -Wpedantic
gcc controller.c -o controller.exe -lm -Wextra -Wpedantic

sudo touch /var/run/agent.pid && sudo chmod 666 /var/run/agent.pid

./agent.exe &
sleep 1
./controller.exe

rm agent.exe
rm controller.exe
sudo rm /var/run/agent.pid
