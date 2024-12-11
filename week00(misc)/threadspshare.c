#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <semaphore.h>
#include <sys/mman.h>
#include <unistd.h>
#include <errno.h>

sem_t *semaphore;

void *thread_function(void *arg) {
    sem_wait(semaphore);
    printf("Thread in process %d acquired the semaphore.\n", getpid());
    sleep(2);
    sem_post(semaphore);
    return NULL;
}

int main() {
    // Create a shared memory region for the semaphore
    semaphore = mmap(NULL, sizeof(sem_t), PROT_READ | PROT_WRITE, MAP_SHARED | MAP_ANONYMOUS, -1, 0);
    if (semaphore == MAP_FAILED) {
        perror("mmap");
        exit(1);
    }

    // Initialize the semaphore in shared memory with pshared set to 1
    if (sem_init(semaphore, 1, 0) == -1) {
        perror("sem_init");
        exit(1);
    }

    pid_t pid = fork();
    if (pid == -1) {
        errno = 10;
        perror("Fork failed");
        exit(10);
    }

    if (pid == 0) { // Child process
        pthread_t thread;
        pthread_create(&thread, NULL, thread_function, NULL);
        pthread_join(thread, NULL);
    } else { // Parent process
        pthread_t thread;
        pthread_create(&thread, NULL, thread_function, NULL);
        pthread_join(thread, NULL);
    }

    sem_destroy(semaphore);
    munmap(semaphore, sizeof(sem_t));
    return 0;
}
