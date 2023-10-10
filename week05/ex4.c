#define _GNU_SOURCE

#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>

// Global variables
int k = 0;     // Next number to check for primality
int c = 0;     // Count of prime numbers found
int n;         // Upper limit
pthread_mutex_t mutex_kc = PTHREAD_MUTEX_INITIALIZER;   // Mutex for k and c

int is_prime(int num) {
    if (num <= 1) return 0;
    if (num <= 3) return 1;
    if (num % 2 == 0 || num % 3 == 0) return 0;
    int i = 5;
    while (i * i <= num) {
        if (num % i == 0 || num % (i + 2) == 0) return 0;
        i += 6;
    }
    return 1;
}

int get_next_k() {
    int current_k;
    pthread_mutex_lock(&mutex_kc);
    current_k = k;
    k++;
    pthread_mutex_unlock(&mutex_kc);
    return current_k;
}

void increment_c() {
    pthread_mutex_lock(&mutex_kc);
    c++;
    pthread_mutex_unlock(&mutex_kc);
}

void* thread_routine(void* arg) {
    while (1) {
        int current_k = get_next_k();
        if (current_k >= n) {
            break; // Exit the thread if k exceeds n
        }
        if (is_prime(current_k)) {
            increment_c();
        }
    }
    return NULL;
}

int main(int argc, char* argv[]) {
    if (argc != 3) {
        printf("Usage: %s <n> <m>\n", argv[0]);
        return 1;
    }

    n = atoi(argv[1]);
    int m = atoi(argv[2]);

    pthread_t threads[m];

    // Create the threads
    for (int i = 0; i < m; i++) {
        pthread_create(&threads[i], NULL, thread_routine, NULL);
    }

    // Join the threads
    for (int i = 0; i < m; i++) {
        pthread_join(threads[i], NULL);
    }

    printf("Number of primes in [0, %d): %d\n", n, c);

    // Clean up
    pthread_mutex_destroy(&mutex_kc);

    return 0;
}
