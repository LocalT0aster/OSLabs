#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

int main(int argc, char* argv[]) {
    if (argc != 2) {
        fprintf(stderr, "Usage: %s <number of forks>\n", argv[0]);
        exit(1);
    }

    int n = atoi(argv[1]);

    for (int i = 0; i < n; ++i) {
        fork();
        sleep(5);
    }
    return 0;
}
