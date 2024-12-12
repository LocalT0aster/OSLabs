#!/bin/bash
# "-Wl,-eentry" provides argument -eentry to linker (ld),
# that rebinds the entrypoint to entry symbol.
# "-nostartfiles" avoids glibc _start function that assumes,
# that the entrypoint symbol is main (otherwise we would need to compile
# modified glibc)
gcc -nostartfiles -Wl,-eentry -o nomain.out nomain.c
./nomain.out
rm nomain.out
