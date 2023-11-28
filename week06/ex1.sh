#!/bin/bash
gcc agent.c -o agent.out -Wextra -Wpedantic
gcc controller.c -o controller.out -Wextra -Wpedantic

sudo touch /var/run/agent.pid && sudo chmod 666 /var/run/agent.pid

./agent.out &
sleep 1
./controller.out

rm agent.out
rm controller.out
sudo rm /var/run/agent.pid
