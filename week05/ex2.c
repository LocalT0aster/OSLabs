#define _GNU_SOURCE

#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <string.h>

typedef struct {
    pthread_t id;
    int i;
    char message[256];
} Thread;

// Thread function
void *print_message(void *ptr) {
    Thread *thread_data = (Thread *)ptr;
    printf("Thread %lu: %s\n", thread_data->id, thread_data->message);
    printf("Thread %d exits\n", thread_data->i + 1);
    pthread_exit(NULL);
}

int main(int argc, char *argv[]) {
    if (argc < 2) {
        printf("Usage: ./ex2.exe <number of threads>\n");
        exit(1);
    }

    int n = atoi(argv[1]);
    Thread threads[n];

    for (int i = 0; i < n; i++) {
        threads[i].i = i;
        sprintf(threads[i].message, "Hello from thread %d", i + 1);

        int ret = pthread_create(&threads[i].id, NULL, print_message, &threads[i]);

        if (ret) {
            printf("Error creating thread. Error code: %d\n", ret);
            exit(-1);
        }

        printf("Thread %d is created\n", i + 1);
        
        // Ensure messages are printed in order
        pthread_join(threads[i].id, NULL);
    }

    return 0;
}
