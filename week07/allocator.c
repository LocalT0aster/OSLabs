// ex1
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>

#define MAX_HEAP_SIZE 10000000

typedef struct {
    unsigned int start;
    unsigned int size;
} Block;

Block memory[MAX_HEAP_SIZE];
int memory_blocks = 0;

void initialize_memory() {
    for (int i = 0; i < MAX_HEAP_SIZE; ++i) {
        memory[i].start = 0;
        memory[i].size = 0;
    }
    memory_blocks = 0;
}

int allocate_first_fit(unsigned int adrs, unsigned int size) {
    unsigned int start = 0;
    while (start + size <= MAX_HEAP_SIZE) {
        int found = 1;
        for (int i = 0; i < memory_blocks; ++i) {
            if (start >= memory[i].start && start < memory[i].start + memory[i].size) {
                start = memory[i].start + memory[i].size;
                found = 0;
                break;
            }
        }
        if (found) {
            memory[memory_blocks].start = start;
            memory[memory_blocks].size = size;
            ++memory_blocks;
            return 1;
        }
    }
    return 0;
}

int allocate_best_fit(unsigned int adrs, unsigned int size) {
    int best_index = -1;
    unsigned int min_fragmentation = MAX_HEAP_SIZE + 1;
    unsigned int start = 0;

    while (start + size <= MAX_HEAP_SIZE) {
        int found = 1;
        unsigned int fragmentation = MAX_HEAP_SIZE - (start + size);
        unsigned int next_start = start + size;  // Initialize to next contiguous block by default

        for (int i = 0; i < memory_blocks; ++i) {
            if (start >= memory[i].start && start < memory[i].start + memory[i].size) {
                if (memory[i].start + memory[i].size > next_start) {  // Update next_start if a larger overlap is found
                    next_start = memory[i].start + memory[i].size;
                }
                found = 0;
            }
        }

        if (found && fragmentation < min_fragmentation) {
            min_fragmentation = fragmentation;
            best_index = start;
        }
        
        start = next_start;  // Move the start to after the last overlapping block
    }

    if (best_index != -1) {
        memory[memory_blocks].start = best_index;
        memory[memory_blocks].size = size;
        ++memory_blocks;
        return 1;
    }
    return 0;
}

int allocate_worst_fit(unsigned int adrs, unsigned int size) {
    int worst_index = -1;
    unsigned int max_fragmentation = 0;
    unsigned int start = 0;

    while (start + size <= MAX_HEAP_SIZE) {
        int found = 1;
        unsigned int fragmentation = MAX_HEAP_SIZE - (start + size);
        unsigned int next_start = start + size;  // Initialize to next contiguous block by default

        for (int i = 0; i < memory_blocks; ++i) {
            if (start >= memory[i].start && start < memory[i].start + memory[i].size) {
                if (memory[i].start + memory[i].size > next_start) {  // Update next_start if a larger overlap is found
                    next_start = memory[i].start + memory[i].size;
                }
                found = 0;
            }
        }

        if (found && fragmentation > max_fragmentation) {
            max_fragmentation = fragmentation;
            worst_index = start;
        }
        
        start = next_start;  // Move the start to after the last overlapping block
    }

    if (worst_index != -1) {
        memory[memory_blocks].start = worst_index;
        memory[memory_blocks].size = size;
        ++memory_blocks;
        return 1;
    }
    return 0;
}

void clear(unsigned int adrs) {
    for (int i = 0; i < memory_blocks; ++i) {
        if (memory[i].start == adrs) {
            memory[i].start = 0;
            memory[i].size = 0;
            --memory_blocks;
            break;
        }
    }
}

void print_memory() {
    for (int i = 0; i < memory_blocks; ++i) {
        if (memory[i].size > 0) {
            printf("Address: %u, Size: %u\n", memory[i].start, memory[i].size);
        }
    }
}

double process_queries(const char *filename, int (*allocate)(unsigned int, unsigned int)) {
    FILE *file = fopen(filename, "r");
    if (!file) {
        perror("Error opening file");
        return -1;
    }

    char command[10];
    unsigned int adrs, size;
    clock_t start_time = clock();

    while (fscanf(file, "%s", command) != EOF) {
        if (strcmp(command, "allocate") == 0) {
            fscanf(file, "%u %u", &adrs, &size);
            if (!allocate(adrs, size)) {
                fprintf(stderr, "Allocation failed for address %u, size %u\n", adrs, size);
            }
        } else if (strcmp(command, "clear") == 0) {
            fscanf(file, "%u", &adrs);
            clear(adrs);
        } else if (strcmp(command, "end") == 0) {
            break;
        }
    }

    clock_t end_time = clock();
    double time_taken = (double)(end_time - start_time) / CLOCKS_PER_SEC;
    fclose(file);

    return time_taken;
}

int main() {
    const char *filename = "queries.txt";

    // First Fit
    initialize_memory();
    double time_taken = process_queries(filename, allocate_first_fit);
    printf("First Fit: %lf queries/second\n", time_taken > 0 ? 1 / time_taken : 0);

    // Best Fit
    initialize_memory();
    time_taken = process_queries(filename, allocate_best_fit);
    printf("Best Fit: %lf queries/second\n", time_taken > 0 ? 1 / time_taken : 0);

    // Worst Fit
    initialize_memory();
    time_taken = process_queries(filename, allocate_worst_fit);
    printf("Worst Fit: %lf queries/second\n", time_taken > 0 ? 1 / time_taken : 0);

    return 0;
}
