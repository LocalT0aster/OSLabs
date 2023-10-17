// ex1
#include <stdio.h>
#include <stdlib.h>
#include <signal.h>
#include <unistd.h>

#define BUF_SIZE 1024

void sig_handler(int signo) {
    if (signo == SIGUSR1) {
        FILE* file = fopen("text.txt", "r");
        if (file) {
            char line[BUF_SIZE];
            printf("\n");
            while (fgets(line, sizeof(line), file)) {
                printf("%s", line);
            }
            fclose(file);
        } else {
            printf("Error reading text.txt\n");
        }
    } else if (signo == SIGUSR2) {
        printf("Process terminating...\n");
        exit(0);
    }
}

int main() {
    setvbuf(stdout, NULL, _IONBF, 0);
    FILE* pid_file = fopen("/var/run/agent.pid", "w");
    if (!pid_file) {
        perror("Failed to open /var/run/agent.pid for writing");
        return 1;
    }
    fprintf(pid_file, "%d", getpid());
    fclose(pid_file);

    if (signal(SIGUSR1, sig_handler) == SIG_ERR)
        printf("Can't catch SIGUSR1\n");
    if (signal(SIGUSR2, sig_handler) == SIG_ERR)
        printf("Can't catch SIGUSR2\n");

    // Print initial text
    sig_handler(SIGUSR1);

    while (1) {
        pause();  // Wait indefinitely for a signal
    }

    return 0;
}
