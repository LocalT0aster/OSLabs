#define _GNU_SOURCE

#include<sys/types.h>
#include<sys/wait.h>
#include<unistd.h>
#include<semaphore.h>
#include<signal.h>
#include<stdio.h>
#include<time.h>

/**
 * @brief Time difference macro function.
 * Used to get real time difference.
 * @param B `[struct timespec]` start time.
*/
#define T_DIFF(B) endtime.tv_nsec - (B).tv_nsec
/**
 * @brief Nanoseconds to Milliseconds macro function.
 * @param T time in `ns`.
*/
#define NS2MS(T) (double)(T) / 1000000.0
/**
 * @brief Clock to Milliseconds macro function.
 * Calculates difference between `current clock` and `begin clock`, and converts to milliseconds
 * @param C start clock in `ticks`
*/
#define CLOCK2MS(C) (((double)(clock() - (C))) / CLOCKS_PER_SEC * 1000.0)

/**
 * @brief Prints I am X message with `PID` & `PPID` of the process.
 * @param str I am X string pointer
*/
void printpid(char* str) {
    printf("%s\nmy pid: %d\nmy parent: %d\n", str, getpid(), getppid());
}

int main() {
    // Get parent clock
    long pclock = clock();

    // Variables declaration
    setvbuf(stdout, NULL, _IONBF, 0); // Disable printf buffering
    char* str; // I am X string
    struct timespec ptime, c1time, c2time, endtime; // Real time variables
    long c1clock, c2clock; // Clock wariables
    pid_t child1, child2; // Children PID variables
    clock_gettime(CLOCK_REALTIME, &ptime); // Get current time

    // Semaphore (so children doesn't fight over output)
    sem_t sm;
    sem_init(&sm, 1, 1); // Semaphore initialization

    // Creation and time measurment of 1st child process
    child1 = fork();
    c1clock = clock();
    if (child1 == 0) {
        clock_gettime(CLOCK_REALTIME, &c1time);
    }

    // Creation and time measurment of 2nd child process
    if (child1 != 0) {
        child2 = fork();
        c2clock = clock();
        if (child2 == 0) {
            clock_gettime(CLOCK_REALTIME, &c2time);
        }
    }

    // Main code part
    if (child1 != 0 && child2 != 0) { // "If I know my children, I am a parent."
        str = "I am parent (P)";

        sem_wait(&sm); // Wait for child 1 to finish
        clock_gettime(CLOCK_REALTIME, &endtime);
        printf("P got semaphore at %ldns\n", T_DIFF(ptime));

        wait(NULL); // Wait for children to die >:)
        clock_gettime(CLOCK_REALTIME, &endtime);
        printf("Children died at %ldns\n", T_DIFF(ptime));

        printpid(str); // Print my PID

        clock_gettime(CLOCK_REALTIME, &endtime);
        printf("P Execution time: %lfms\n(%lfms in real time or %ldns in total)\n", CLOCK2MS(pclock), NS2MS(T_DIFF(ptime)), T_DIFF(ptime));

        // Safely exit
        sem_destroy(&sm);
        return 0;
    } else if (child1 == 0) { // "If I don't know 1st child, I am 1st child."
        str = "I am child 1 (C1)";

        printpid(str); // Print my PID

        clock_gettime(CLOCK_REALTIME, &endtime);
        printf("C1 Execution time: %lfms\n(%lfms in real time or %ldns from P start)\n", CLOCK2MS(c1clock), NS2MS(T_DIFF(c1time)), T_DIFF(ptime));

        sem_post(&sm); // Tell C2 and P that I'm done.
        return 0;
    } else if (child2 == 0) { // "If I don't know 2nd child, I am 2nd child."
        str = "I am child 2 (C2)";

        sem_wait(&sm); // Wait for C1 to get things done
        clock_gettime(CLOCK_REALTIME, &endtime);
        printf("C2 got semaphore at %ldns\n", T_DIFF(ptime));

        printpid(str); // Print my PID

        clock_gettime(CLOCK_REALTIME, &endtime);
        printf("C2 Execution time: %lfms\n(%lfms in real time or %ldns from P start)\n", CLOCK2MS(c2clock), NS2MS(T_DIFF(c2time)), T_DIFF(ptime));
        return 0;
    }
    return 1;
}
