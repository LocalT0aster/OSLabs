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
        fprintf(stderr, "Usage: %s <number of subscribers>\n", argv[0]);
        exit(EXIT_FAILURE);
    }

    int n = atoi(argv[1]);
    char path[1024];
    char msg[MSG_SIZE];

    struct stat st = {0};
    if (stat("/tmp/ex1", &st) == -1) {
        mkdir("/tmp/ex1", 0700);
    }

    int pipes[n][2];

    for (int i = 0; i < n; i++) {
        if (pipe(pipes[i]) == -1) {
            perror("Pipe creation failed");
            exit(EXIT_FAILURE);
        }

        pid_t pid = fork();

        if (pid < 0) {
            perror("Fork failed");
            exit(EXIT_FAILURE);
        }

        if (pid == 0) {  // Child process
            close(pipes[i][1]);  // Close unused write end

            snprintf(path, sizeof(path), "%s%d", BASE_PATH, i+1);
            mkfifo(path, 0666);

            int fd = open(path, O_WRONLY);

            while (read(pipes[i][0], msg, MSG_SIZE) > 0) {
                write(fd, msg, MSG_SIZE);
            }

            close(pipes[i][0]);
            close(fd);
            exit(0);
        }
    }

    while (1) {
        printf("Enter message: ");
        fgets(msg, MSG_SIZE, stdin);

        for (int i = 0; i < n; i++) {
            write(pipes[i][1], msg, MSG_SIZE);
        }
        if(msg[0] == '\n')
            break;
    }

    return 0;
}
