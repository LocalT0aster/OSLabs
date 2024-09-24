#define _GNU_SOURCE

#include<sys/types.h>
#include<sys/wait.h>
#include<unistd.h>
#include<semaphore.h>
#include<signal.h>
#include<stdio.h>
#include<time.h>

int main() {
    int n = 4;
    pid_t c = fork();
    if (c != 0) {
        printf("Hello from Parent [%d - %d - %p]\n", getpid(), n, &n);
    } else {
        printf("Hello from Child [%d - %d - %p]\n", getpid(), n, &n);
    }
}