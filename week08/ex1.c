// ex1

#define _GNU_SOURCE

#include <stdio.h>
#include <stdlib.h>
#include <fcntl.h>
#include <string.h>
#include <sys/mman.h>
#include <unistd.h>
#include <ctype.h>

#define PASS_SIZE 13
#define ALLOCATED_SIZE 16

int main() {
    // Write its pid in /tmp/ex1.pid
    FILE *f = fopen("/tmp/ex1.pid", "w");
    if (f == NULL) {
        perror("Error opening /tmp/ex1.pid");
        exit(1);
    }
    fprintf(f, "%d", getpid());
    fflush(f);
    fclose(f);

    // Generate a random password using /dev/urandom
    char* password = calloc(ALLOCATED_SIZE, sizeof(char));
    snprintf(password, 6, "pass:"); // Set the initial "pass:"
    int fd = open("/dev/urandom", O_RDONLY);
    if (fd == -1) {
        perror("Error opening /dev/urandom");
        exit(1);
    }
    char c;
    int len = 5; // start after "pass:"
    while (len < PASS_SIZE) {
        if (read(fd, &c, 1) && isprint(c)) {
            password[len++] = c;
        }
    }
    password[PASS_SIZE] = '\0';
    close(fd);

    printf("%s\n", password);

    // Store the password in the memory using mmap
    char *shared_mem;
    shared_mem = mmap(NULL, ALLOCATED_SIZE, PROT_READ | PROT_WRITE, MAP_SHARED | MAP_ANONYMOUS, -1, 0);
    if (shared_mem == MAP_FAILED) {
        perror("Error with mmap");
        exit(1);
    }
    memcpy(shared_mem, password, ALLOCATED_SIZE);

    // Infinite loop
    while (1) {
        pause();
    }

    return 0;
}
