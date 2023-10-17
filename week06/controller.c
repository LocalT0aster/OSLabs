// ex1
#include <stdio.h>
#include <stdlib.h>
#include <signal.h>
#include <unistd.h>
#include <string.h>
#include <sys/types.h>

pid_t get_agent_pid() {
    FILE *pid_file = fopen("/var/run/agent.pid", "r");
    if (!pid_file) return -1;
    pid_t pid;
    fscanf(pid_file, "%d", &pid);
    fclose(pid_file);
    return pid;
}

void terminate_handler(int signo) {
    pid_t pid = get_agent_pid();
    if (pid > 0) kill(pid, SIGTERM);
    exit(0);
}

int main() {
    setvbuf(stdout, NULL, _IONBF, 0);
    pid_t agent_pid = get_agent_pid();
    if (agent_pid == -1) {
        printf("Error: No agent found.\n");
        return 1;
    }
    printf("Agent found.\n");

    if (signal(SIGINT, terminate_handler) == SIG_ERR) {
        printf("Can't catch SIGINT\n");
        return 1;
    }

    while (1) {
        printf("Choose a command {\"read\", \"exit\", \"stop\", \"continue\"} to send to the agent: ");
        char command[10];
        scanf("%9s", command);

        if (strcmp(command, "read") == 0) {
            kill(agent_pid, SIGUSR1);
        } else if (strcmp(command, "exit") == 0) {
            kill(agent_pid, SIGUSR2);
            break;
        } else if (strcmp(command, "stop") == 0) {
            kill(agent_pid, SIGSTOP);
        } else if (strcmp(command, "continue") == 0) {
            kill(agent_pid, SIGCONT);
        } else {
            printf("Invalid command.\n");
        }
    }

    return 0;
}
