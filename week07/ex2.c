// ex2

#define _GNU_SOURCE

#include <stdio.h>
#include <stdlib.h>
#include <fcntl.h>
#include <unistd.h>
#include <sys/mman.h>
#include <sys/types.h>
#include <pthread.h>
#include <ctype.h>

#define FILE_SIZE (500L * 1024 * 1024)
#define LINE_LENGTH 1023
#define THREAD_N 4
#define READ_BUF_SIZE 4096

typedef struct {
    char *addr;
    size_t size;
    long capitalCount;
} ThreadData;

char* addr;

void generate() {
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

    size_t generated = 0;
    char read_buf[READ_BUF_SIZE];
    char write_buf[LINE_LENGTH];
    size_t write_index = 0;

    while (generated < FILE_SIZE) {
        ssize_t n = read(fd, read_buf, READ_BUF_SIZE);
        if (n <= 0) {
            perror("Read from /dev/urandom failed");
            close(fd);
            fclose(f);
            exit(EXIT_FAILURE);
        }

        for (ssize_t i = 0; i < n && generated < FILE_SIZE; i++) {
            if (isprint(read_buf[i])) {
                write_buf[write_index++] = read_buf[i];
                if (write_index == LINE_LENGTH) {
                    fwrite(write_buf, 1, LINE_LENGTH, f);
                    fputc('\n', f);
                    generated += LINE_LENGTH + 1;
                    write_index = 0;
                }
            }
        }
    }

    fflush(f);
    fclose(f);
    close(fd);
    return;
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
    if (msync(addr, FILE_SIZE, MS_SYNC) == -1) {
        perror("msync failed");
    }
    return NULL;
}

int main() {
    // Generate the text.txt file
    generate();

    int fd = open("text.txt", O_RDWR);
    if (fd == -1) {
        perror("Failed to open text.txt");
        exit(EXIT_FAILURE);
    }

    addr = mmap(NULL, FILE_SIZE, PROT_READ | PROT_WRITE, MAP_SHARED, fd, 0);
    if (addr == MAP_FAILED) {
        perror("Failed to mmap text.txt");
        close(fd);
        exit(EXIT_FAILURE);
    }

    long sz = sysconf(_SC_PAGESIZE);
    size_t chunk_size = 1024 * sz;
    long total_chunks = FILE_SIZE / chunk_size;
    long chunks_per_thread = total_chunks / THREAD_N;
    long remainder_chunks = total_chunks % THREAD_N; // Remainder chunks to be distributed

    pthread_t threads[THREAD_N];
    ThreadData data[THREAD_N];
    for (int i = 0; i < THREAD_N; i++) {
        data[i].addr = addr + (i * chunks_per_thread * chunk_size);
        data[i].size = chunks_per_thread * chunk_size;
        data[i].capitalCount = 0;

        // Distribute remainder chunks to the last thread
        if (i == THREAD_N - 1) {
            data[i].size += remainder_chunks * chunk_size;
        }
        
        pthread_create(&threads[i], NULL, count_and_replace, &data[i]);
    }

    long totalCapitals = 0;
    for (int i = 0; i < THREAD_N; i++) {
        pthread_join(threads[i], NULL);
        totalCapitals += data[i].capitalCount;
    }

    printf("Total capital letters: %ld\n", totalCapitals);

    if (msync(addr, FILE_SIZE, MS_SYNC) == -1) {
        perror("msync failed");
    }
    munmap(addr, FILE_SIZE);
    close(fd);

    return 0;
}
