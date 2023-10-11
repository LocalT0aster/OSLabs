#!/bin/bash
gcc agent.c -o agent.exe -lm -Wextra -Wpedantic
gcc controller.c -o controller.exe -lm -Wextra -Wpedantic



rm agent.exe
rm controller.exe
