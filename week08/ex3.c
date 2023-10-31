// ex3

#define _GNU_SOURCE

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/time.h>
#include <sys/resource.h>
#include <unistd.h>

#define MB 0x100000

int main() {
    for (int i = 0; i < 10; i++) {
        // Allocate 10 MB of memory
        char *memory = (char *)malloc(10 * MB);
        if (!memory) {
            fprintf(stderr, "Memory allocation failed\n");
            return 1;
        }

        // Fill the memory with zeros
        memset(memory, 0, 10 * MB);

        // Get and print memory usage
        struct rusage rusage_info;
        if (getrusage(RUSAGE_SELF, &rusage_info) == 0) {
            printf("Memory usage: %ld kB\n", rusage_info.ru_maxrss);
        } else {
            fprintf(stderr, "Failed to get resource usage\n");
        }

        // Sleep for 1 second
        sleep(1);
    }
    return 0;
}
