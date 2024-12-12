#include <stdio.h>
#include <stdlib.h>

int entry(){ // No main() function
    printf("lol\n");
    exit(0); // Segfaults on return 0, we need to call exit explicitly
}
