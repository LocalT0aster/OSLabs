#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <fcntl.h>
#include <sys/stat.h>
#include <sys/types.h>

#define MSG_SIZE 1024
#define BASE_PATH "/tmp/ex1/s"

int main(int argc, char *argv[]) {
    if (argc != 2) {
        fprintf(stderr, "Usage: %s <subscriber id>\n", argv[0]);
        exit(EXIT_FAILURE);
    }

    int id = atoi(argv[1]);
    char path[1024];
    char msg[MSG_SIZE];
    int fd;

    snprintf(path, sizeof(path), "%s%d", BASE_PATH, id);
    fd = open(path, O_RDONLY);

    while (1) {
        read(fd, msg, MSG_SIZE);
        printf("Subscriber %d received: %s", id, msg);
        if(msg[0] == '\n')
            break;
    }

    close(fd);

    return 0;
}
