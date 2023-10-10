#define _GNU_SOURCE

#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>

typedef struct {
    int start;
    int end;
} Range;

int is_prime(int n) {
    if (n <= 1) return 0;
    if (n <= 3) return 1;
    if (n % 2 == 0 || n % 3 == 0) return 0;
    int i = 5;
    while (i * i <= n) {
        if (n % i == 0 || n % (i + 2) == 0) return 0;
        i += 6;
    }
    return 1;
}

void* count_primes(void* arg) {
    Range* r = (Range*) arg;
    int count = 0;
    for (int i = r->start; i < r->end; i++) {
        if (is_prime(i)) {
            count++;
        }
    }
    return (void*)(long)count;
}

int main(int argc, char* argv[]) {
    if (argc != 3) {
        printf("Usage: %s <n> <m>\n", argv[0]);
        return 1;
    }

    int n = atoi(argv[1]);
    int m = atoi(argv[2]);
    int segment = n / m;

    pthread_t threads[m];
    Range ranges[m];
    int total_primes = 0;

    for (int i = 0; i < m; i++) {
        ranges[i].start = i * segment;
        ranges[i].end = (i == m - 1) ? n : (i + 1) * segment;
        pthread_create(&threads[i], NULL, count_primes, &ranges[i]);
    }

    for (int i = 0; i < m; i++) {
        void* count;
        pthread_join(threads[i], &count);
        total_primes += (int)(long)count;
    }

    printf("Number of primes in [0, %d): %d\n", n, total_primes);

    return 0;
}
