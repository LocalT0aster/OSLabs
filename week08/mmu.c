// ex2
#define _GNU_SOURCE

#include <stdio.h>
#include <stdlib.h>
#include <signal.h>
#include <string.h>
#include <fcntl.h>
#include <sys/mman.h>
#include <unistd.h>

#define PAGE_TABLE_SIZE 256 // Assuming a maximum of 256 pages
#define PAGETABLE_PATH "/ex2_pagetable"

typedef struct {
    int valid;
    int frame;
    int dirty;
    int referenced;
} page_table_entry;

// Global variables

page_table_entry* page_table = NULL;
int num_pages;

pid_t pager_pid;

// Function to signal pager for page fault
void signal_pager_for_page_fault(int page_number) {
    // Send SIGUSR1 to the pager to indicate a page fault
    kill(pager_pid, SIGUSR1);

    // Wait for the pager to handle the page fault
    pause(); // This should be replaced with sigsuspend() in a full implementation
}

// Function to print the page table
void print_page_table() {
    printf("Page Table:\n");
    for (int i = 0; i < num_pages; i++) {
        printf("PTE %d - valid: %d, frame: %d, dirty: %d, referenced: %d\n",
            i, page_table[i].valid, page_table[i].frame, page_table[i].dirty, page_table[i].referenced);
    }
}

// Function to handle memory access
void handle_memory_access(char* access) {
    int page_number;
    char mode;

    // Parse the access string (format "Rn" or "Wn" where n is the page number)
    if (sscanf(access, "%c%d", &mode, &page_number) != 2 || page_number >= num_pages) {
        fprintf(stderr, "Invalid access string: %s\n", access);
        exit(EXIT_FAILURE);
    }

    // Check if the page is valid (in RAM)
    if (!page_table[page_number].valid) {
        // Page fault, signal the pager
        signal_pager_for_page_fault(page_number);
    }

    // Set the referenced flag
    page_table[page_number].referenced = 1;

    // If it's a write access, set the dirty flag
    if (mode == 'W') {
        page_table[page_number].dirty = 1;
    }

    // Print the page table state after this memory access
    print_page_table();
}

// Function to initialize page table using a temporary file
void initialize_page_table() {
    // Open a temporary file for the page table
    int fd = open(PAGETABLE_PATH, O_CREAT | O_RDWR, 0666);
    if (fd == -1) {
        perror("open");
        exit(EXIT_FAILURE);
    }

    // Size the file
    if (ftruncate(fd, num_pages * sizeof(page_table_entry)) == -1) {
        perror("ftruncate");
        close(fd);
        exit(EXIT_FAILURE);
    }

    // Map the file into memory
    page_table = mmap(NULL, num_pages * sizeof(page_table_entry), PROT_READ | PROT_WRITE, MAP_SHARED, fd, 0);
    if (page_table == MAP_FAILED) {
        perror("mmap");
        close(fd);
        exit(EXIT_FAILURE);
    }

    // The file descriptor is no longer needed after the memory is mapped
    close(fd);
}

void write_pid() {
    // Write the MMU's PID to a file for the pager to read
    FILE* pid_file = fopen(".mmu.pid", "w");
    if (!pid_file) {
        perror("Error opening MMU PID file for writing");
        exit(EXIT_FAILURE);
    }
    fprintf(pid_file, "%d", getpid());
    fflush(pid_file);
    fclose(pid_file);
}

void sigcont_handler(int signum) {}

int main(int argc, char* argv[]) {
    // Ensure the correct number of arguments
    if (argc < 4) {
        fprintf(stderr, "Usage: %s <num_pages> <access_sequence> <pager_pid>\n", argv[0]);
        return EXIT_FAILURE;
    }

    // Parse the number of pages
    num_pages = atoi(argv[1]);
    if (num_pages <= 0 || num_pages > PAGE_TABLE_SIZE) {
        fprintf(stderr, "Invalid number of pages. Must be between 1 and %d.\n", PAGE_TABLE_SIZE);
        return EXIT_FAILURE;
    }

    // Parse the pager PID
    pager_pid = (pid_t)atoi(argv[argc - 1]);
    if (pager_pid <= 0) {
        fprintf(stderr, "Invalid pager PID.\n");
        return EXIT_FAILURE;
    }

    // Bind SIGCONT to sigcont_handler()
    struct sigaction sa;
    memset(&sa, 0, sizeof(sa));
    sigemptyset(&sa.sa_mask);
    sa.sa_flags = 0;
    sa.sa_handler = sigcont_handler;
    sigaction(SIGCONT, &sa, NULL);

    // PID exchange mmu side
    write_pid();
    sleep(1);
    kill(pager_pid, SIGCONT);
    printf("PID write done, waiting for pager.\n");
    pause();
    printf("PID exchange successfull\n");

    // Initialize page table
    // Using an anonymous mapping for simplicity
    page_table = mmap(NULL, num_pages * sizeof(page_table_entry), PROT_READ | PROT_WRITE, MAP_SHARED | MAP_ANONYMOUS, -1, 0);
    if (page_table == MAP_FAILED) {
        perror("mmap");
        return EXIT_FAILURE;
    }

    // Initialize all page table entries to 0
    memset(page_table, 0, num_pages * sizeof(page_table_entry));

    // Initialize shared memory for the page table
    initialize_page_table();

    // Process the memory access sequence
    for (int i = 2; i < argc - 1; i++) {
        handle_memory_access(argv[i]);
    }

    // Signal pager at the end of memory accesses
    kill(pager_pid, SIGUSR2); // Let's assume SIGUSR2 is used for termination signal

    // Cleanup
    munmap(page_table, num_pages * sizeof(page_table_entry));
    shm_unlink(PAGETABLE_PATH); // Remove the shared memory object

    return EXIT_SUCCESS;
}
