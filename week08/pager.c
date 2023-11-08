// ex2
#define _GNU_SOURCE

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <signal.h>
#include <unistd.h>
#include <time.h>
#include <sys/types.h>
#include <sys/mman.h>
#include <sys/stat.h>
#include <fcntl.h>

#define MAX_FRAMES 256
#define PAGE_SIZE 8
#define SHARED_MEMORY_OBJECT "/tmp/ex2/pagetable"

typedef struct {
    int valid;
    int frame;
    int dirty;
    int referenced;
} page_table_entry;

char ram[MAX_FRAMES][PAGE_SIZE];
char disk[MAX_FRAMES][PAGE_SIZE];
page_table_entry* page_table = NULL;

int num_frames, num_pages;
int num_disk_accesses = 0;
int shm_fd; // File descriptor for the shared memory object

pid_t mmu_pid;

// Function to find a free frame, or return -1 if none are free
int find_free_frame() {
    for (int i = 0; i < num_frames; i++) {
        if (!page_table[i].valid) {
            return i;
        }
    }
    return -1;
}

// Function to select a victim frame for replacement
int select_victim_frame() {
    // Simple random page replacement policy
    return rand() % num_frames;
}

// Function to handle incoming page faults
void page_fault_handler(int sig) {
    int page_number = -1; // This would typically be passed via IPC mechanism

    // Find a free frame
    int frame_number = find_free_frame();

    // If no free frame is available, select a victim frame to replace
    if (frame_number == -1) {
        frame_number = select_victim_frame();
        // If the victim frame is dirty, write it back to disk
        if (page_table[frame_number].dirty) {
            // Simulate writing the frame back to disk
            num_disk_accesses++;
        }
    }

    // Load the new page into the selected frame
    // Simulate reading the page from disk
    strncpy(ram[frame_number], disk[page_number], PAGE_SIZE);
    num_disk_accesses++;

    // Update the page table
    page_table[page_number].valid = 1;
    page_table[page_number].frame = frame_number;

    // Send a signal back to the MMU to continue
    kill(mmu_pid, SIGCONT);
}

// Function to handle MMU termination signal
void termination_handler(int sig) {
    // Print the total number of disk accesses
    printf("Total number of disk accesses: %d\n", num_disk_accesses);

    // Cleanup and termination
    // ...

    exit(EXIT_SUCCESS);
}

// Function to initialize disk with random messages
void initialize_disk() {
    for (int i = 0; i < num_pages; i++) {
        snprintf(disk[i], PAGE_SIZE, "Page%d", i);
    }
}

void initialize_shared_memory() {
    shm_fd = shm_open(SHARED_MEMORY_OBJECT, O_CREAT | O_RDWR, 0666);
    if (shm_fd == -1) {
        perror("shm_open");
        exit(EXIT_FAILURE);
    }
    ftruncate(shm_fd, num_pages * sizeof(page_table_entry));
    page_table = mmap(0, num_pages * sizeof(page_table_entry), PROT_READ | PROT_WRITE, MAP_SHARED, shm_fd, 0);
    if (page_table == MAP_FAILED) {
        perror("mmap");
        exit(EXIT_FAILURE);
    }
}


// Function to read the MMU's PID from a file
void read_mmu_pid() {
    FILE* pid_file = fopen(".mmu.pid", "r");
    if (!pid_file) {
        perror("Error opening MMU PID file");
        exit(EXIT_FAILURE);
    }
    if (fscanf(pid_file, "%d", &mmu_pid) != 1) {
        perror("Error reading MMU PID file");
        fclose(pid_file);
        exit(EXIT_FAILURE);
    }
    fclose(pid_file);
}

int main(int argc, char* argv[]) {
    // Ensure the correct number of arguments
    if (argc != 3) {
        fprintf(stderr, "Usage: %s <num_pages> <num_frames>\n", argv[0]);
        return EXIT_FAILURE;
    }
    read_mmu_pid();

    // Parse the number of pages and frames
    num_pages = atoi(argv[1]);
    num_frames = atoi(argv[2]);
    if (num_pages <= 0 || num_pages > MAX_FRAMES || num_frames <= 0 || num_frames > MAX_FRAMES) {
        fprintf(stderr, "Invalid number of pages or frames\n");
        return EXIT_FAILURE;
    }

    // Initialize shared memory for the page table
    initialize_shared_memory();

    // Initialize the disk with random messages
    initialize_disk();

    // Initialize random number generator for victim frame selection
    srand(time(NULL));

    // Initialize the page table
    page_table = (page_table_entry*)calloc(num_pages, sizeof(page_table_entry));
    if (page_table == NULL) {
        perror("Failed to allocate page table");
        exit(EXIT_FAILURE);
    }

    // Setup signal handlers for page fault and termination
    struct sigaction sa;
    sa.sa_handler = page_fault_handler;
    sigemptyset(&sa.sa_mask);
    sa.sa_flags = 0;
    sigaction(SIGUSR1, &sa, NULL);

    sa.sa_handler = termination_handler;
    sigaction(SIGUSR2, &sa, NULL);

    while (1) {
        pause(); // Wait for a signal
    }

    // Cleanup would be here if necessary

    return EXIT_SUCCESS;
}
