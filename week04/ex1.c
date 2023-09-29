#define _GNU_SOURCE

#include<sys/types.h>
#include<sys/wait.h>
#include<stdio.h>
// #include<stdlib.h>
#include<unistd.h>
#include<semaphore.h>
#include<signal.h>
#include<time.h>

#define T_DIFF(B) endtime.tv_nsec - (B).tv_nsec
#define NS2MS(T) (double)(T) / 1000000.0
#define CLOCK2MS(C) (((double)(clock() - (C))) / CLOCKS_PER_SEC * 1000.0)

void printpid(char* str) {
    printf("%s\nmy pid: %d\nmy parent: %d\n", str, getpid(), getppid());
}

int main() {
    long pclock = clock();
    
    setvbuf(stdout, NULL, _IONBF, 0); // Disable printf buffering
    char* str;
    struct timespec ptime, c1time, c2time, endtime;
    long c1clock, c2clock;
    clock_gettime(CLOCK_REALTIME, &ptime);

    pid_t child1, child2;
    sem_t sm;
    sem_init(&sm, 1, 1);

    child1 = fork();
    c1clock = clock();
    if (child1 == 0) {
        clock_gettime(CLOCK_REALTIME, &c1time);
    }

    if (child1 != 0) {
        child2 = fork();
        c2clock = clock();
        if (child2 == 0) {
            clock_gettime(CLOCK_REALTIME, &c2time);
        }
    }

    if (child1 != 0 && child2 != 0) {
        str = "I am parent (P)";

        sem_wait(&sm);
        clock_gettime(CLOCK_REALTIME, &endtime);
        printf("P got semaphore at %ldns\n", T_DIFF(ptime));

        wait(NULL);
        clock_gettime(CLOCK_REALTIME, &endtime);
        printf("Children died at %ldns\n", T_DIFF(ptime));
        printpid(str);

        clock_gettime(CLOCK_REALTIME, &endtime);
        printf("P Execution time: %lfms\n(%lfms in real time or %ldns in total)\n", CLOCK2MS(pclock), NS2MS(T_DIFF(ptime)), T_DIFF(ptime));

        sem_destroy(&sm);
        return 0;
    } else if (child1 == 0) {
        str = "I am child 1 (C1)";

        printpid(str);

        clock_gettime(CLOCK_REALTIME, &endtime);
        printf("C1 Execution time: %lfms\n(%lfms in real time or %ldns from P start)\n", CLOCK2MS(c1clock), NS2MS(T_DIFF(c1time)), T_DIFF(ptime));

        sem_post(&sm);
        return 0;
    } else if (child2 == 0) {
        str = "I am child 2 (C2)";

        sem_wait(&sm);
        clock_gettime(CLOCK_REALTIME, &endtime);
        printf("C2 got semaphore at %ldns\n", T_DIFF(ptime));

        printpid(str);
        clock_gettime(CLOCK_REALTIME, &endtime);
        printf("C2 Execution time: %lfms\n(%lfms in real time or %ldns from P start)\n", CLOCK2MS(c2clock), NS2MS(T_DIFF(c2time)), T_DIFF(ptime));
        return 0;
    }
    return 1;
}
