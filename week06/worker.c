// ex2
#define _GNU_SOURCE
#define FILE_OUTPUT

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <signal.h>
#include <time.h>
#include <stdbool.h>
#include <sys/types.h>

// 6 digits for big triangular numbers like 113050
#define TRI_BASE 1000000

// current process pid (which executed this program)
pid_t pid;

// current process idx (starts from 0)
int process_idx;

// number of triangular numbers found so far
long tris;

bool is_triangular(long n) {
    for (long i = 1; i <= n; i++) {
        if (i * (i + 1) == 2 * n) {
            return true;
        }
    }
    return false;
}

void signal_handler(int signum) {
    // print info about number of triangulars found.
    printf("Process %d (PID=<%d>): count of triangulars found so far is %ld\n", process_idx, pid, tris);
    fflush(stdout);

    switch (signum) {
    case SIGTSTP:
        // pause the process indefinitely
        printf("Process %d: stopping....\n", process_idx);
        fflush(stdout);
        pause();
        break;
    case SIGCONT:
        // continue the process
        printf("Process %d: resuming....\n", process_idx);
        fflush(stdout);
        return;
    case SIGTERM:
        // terminate the process
        printf("Process %d: terminating....\n", process_idx);
        fflush(stdout);
        exit(0);
    default:
        break;
    }
}

long big_n() {
    time_t t;
    long n = 0;
    srand((unsigned)time(&t));
    while (n < TRI_BASE)
        n += rand();
    return n % TRI_BASE;
}

int main(int argc, char* argv[]) {
    if (argc < 2) {
        fprintf(stderr, "Usage: %s <process_idx>\n", argv[0]);
        exit(1);
    }
    // Get the process_idx from argv
    process_idx = atoi(argv[1]);

#ifdef FILE_OUTPUT
    int console_stdout = dup(STDOUT_FILENO);
    if (console_stdout == -1) {
        perror("Failed to duplicate stdout");
        return 1;
    }
    FILE* fp = freopen("scheduler.log", "a", stdout);
    if (!fp) {
        perror("Failed to redirect stdout to file");
        return 1;
    }
#endif

    pid = getpid();

    // Register the signals
    signal(SIGTSTP, signal_handler);
    signal(SIGCONT, signal_handler);
    signal(SIGTERM, signal_handler);

    long next_n = big_n() + 1;
    printf("Process %d (PID=<%d>): has been started\n", process_idx, pid);
    printf("Process %d (PID=<%d>): will find the next triangular number from [%ld, inf)\n", process_idx, pid, next_n);
    fflush(stdout);
    tris = 0;
    while (true) {
        if (is_triangular(next_n)) {
            printf("Process %d (PID=<%d>): I found this triangular number %ld\n", process_idx, pid, next_n);
            fflush(stdout);
            tris++;
        }
        next_n++;
    }
    
#ifdef FILE_OUTPUT
    fflush(stdout);
    fclose(stdout);
    if (dup2(console_stdout, STDOUT_FILENO) == -1) {
        perror("Failed to restore stdout");
        return 1;
    }
    close(console_stdout);
#endif
    return 0;
}
