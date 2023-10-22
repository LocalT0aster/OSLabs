// ex2

#define _GNU_SOURCE

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <fcntl.h>
#include <unistd.h>
#include <sys/mman.h>
#include <sys/types.h>
#include <pthread.h>
#include <ctype.h>

#define FILE_SIZE (500L * 1024 * 1024)  // 500 MiB
#define LINE_LENGTH 1024
#define THREAD_N 4

typedef struct {
    char *addr;
    size_t size;
    long capitalCount;
} ThreadData;

void *generation(void *arg) {
    int fd = open("/dev/urandom", O_RDONLY);
    if (fd == -1) {
        perror("Failed to open /dev/urandom");
        exit(EXIT_FAILURE);
    }

    FILE *f = fopen("text.txt", "a");
    if (!f) {
        perror("Failed to open text.txt for appending");
        close(fd);
        exit(EXIT_FAILURE);
    }

    size_t size_to_generate = FILE_SIZE / THREAD_N;
    size_t generated = 0;
    char buf[LINE_LENGTH];

    while (generated < size_to_generate) {
        size_t i = 0;
        while (i < sizeof(buf)) {
            ssize_t n = read(fd, &buf[i], 1);
            if (n == -1) {
                perror("Read from /dev/urandom failed");
                close(fd);
                fclose(f);
                exit(EXIT_FAILURE);
            }
            if (isprint(buf[i])) {
                i++;
            }
        }
        fwrite(buf, 1, sizeof(buf), f);
        fputc('\n', f);
        generated += sizeof(buf) + 1;
    }

    fclose(f);
    close(fd);
    return NULL;
}

void *count_and_replace(void *arg) {
    ThreadData *data = (ThreadData *)arg;
    char *end = data->addr + data->size;

    for (char *p = data->addr; p < end; p++) {
        if (isupper(*p)) {
            data->capitalCount++;
            *p = tolower(*p);
        }
    }

    return NULL;
}

int main() {
    // Generate the text.txt file in parallel
    pthread_t threads[THREAD_N];
    for (int i = 0; i < THREAD_N; i++) {
        pthread_create(&threads[i], NULL, generation, NULL);
    }
    for (int i = 0; i < THREAD_N; i++) {
        pthread_join(threads[i], NULL);
    }

    int fd = open("text.txt", O_RDWR);
    if (fd == -1) {
        perror("Failed to open text.txt");
        exit(EXIT_FAILURE);
    }

    char *addr = mmap(NULL, FILE_SIZE, PROT_READ | PROT_WRITE, MAP_SHARED, fd, 0);
    if (addr == MAP_FAILED) {
        perror("Failed to mmap text.txt");
        close(fd);
        exit(EXIT_FAILURE);
    }

    long sz = sysconf(_SC_PAGESIZE);
    size_t chunk_size = 1024 * sz;

    ThreadData data[THREAD_N];
    for (int i = 0; i < THREAD_N; i++) {
        data[i].addr = addr + (i * chunk_size * (FILE_SIZE / (chunk_size * THREAD_N)));
        data[i].size = chunk_size * (FILE_SIZE / (chunk_size * THREAD_N));
        data[i].capitalCount = 0;
        pthread_create(&threads[i], NULL, count_and_replace, &data[i]);
    }

    long totalCapitals = 0;
    for (int i = 0; i < THREAD_N; i++) {
        pthread_join(threads[i], NULL);
        totalCapitals += data[i].capitalCount;
    }

    printf("Total capital letters: %ld\n", totalCapitals);

    munmap(addr, FILE_SIZE);
    close(fd);

    return 0;
}
