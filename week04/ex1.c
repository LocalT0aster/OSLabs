#include<sys/types.h>
#include<stdio.h>
#include<stdlib.h>
#include<unistd.h>
#include<semaphore.h>
#include<signal.h>
#include<time.h>

void printpid(char *str) {
    printf("%s\nmy pid: %d\nmy parent: %d\n", str, getpid(), getppid());
}

int main() {
    setvbuf(stdout, NULL, _IONBF, 0); // Disable printf buffering
    char* str;
    long ptimer = clock();
    sem_t* sm = calloc(2, sizeof(sem_t));
    sem_init(&sm[0], 1, 1);
    sem_init(&sm[1], 1, 2);

    long c1timer;
    pid_t child1 = fork();
    if(child1 == 0)
        c1timer = clock();

    pid_t child2;
    long c2timer;
    if (child1 != 0) {
        child2 = fork();
        if (child2 == 0)
            c2timer = clock();
    }

    if (child1 != 0 && child2 != 0) {
        str = "I am parent";

        sem_wait(&sm[0]);
        printf("P got sm 0\n");

        sem_wait(&sm[1]);
        printf("P got sm 1\n");

        printpid(str);

        printf("Execution time: %ldms, started at %ld\n", clock() - ptimer, ptimer);

        sem_destroy(&sm[0]);
        sem_destroy(&sm[1]);
        free(sm);
        return 0;
    }
    if (child1 == 0) {
        str = "I am child 1";

        printpid(str);

        printf("Execution time: %ldms, started at %ld\n", clock() - c1timer, c1timer);

        sem_post(&sm[0]);
        return 0;
    } else if (child2 == 0) {
        str = "I am child 2";

        sem_wait(&sm[0]);
        printf("C2 got sm 0\n");

        printpid(str);

        printf("Execution time: %ldms, started at %ld\n", clock() - c2timer, c2timer);

        sem_post(&sm[1]);
        return 0;
    }
    return 0;
}
