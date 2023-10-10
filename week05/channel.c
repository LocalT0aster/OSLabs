#define _GNU_SOURCE

#include <stdio.h>
#include <unistd.h>
#include <semaphore.h>
#include <sys/mman.h>
#include <sys/types.h>
#include <sys/wait.h>

#define MSG_SIZE 1024

int main() {
    int pipefd[2];
    char msg[MSG_SIZE];
    pid_t pid;

    // Create a shared memory region for the semaphore
    sem_t* sem = mmap(NULL, sizeof(sem_t), PROT_READ | PROT_WRITE, MAP_SHARED | MAP_ANONYMOUS, -1, 0);
    if (sem == MAP_FAILED) {
        perror("mmap failed");
        return 1;
    }

    // Initialize the semaphore in shared memory with pshared set to 1
    if (sem_init(sem, 1, 0) == -1) {
        perror("sem_init failed");
        return 1;
    }

    // Сreate pipe
    if (pipe(pipefd) == -1) {
        perror("Pipe creation failed");
        return 1;
    }

    // Аork process
    if ((pid = fork()) == -1) {
        perror("Fork failed");
        return 1;
    }

    if (pid == 0) {  // Child process (subscriber)
        close(pipefd[1]);  // Close unused write end
        while (read(pipefd[0], msg, MSG_SIZE) > 0) {
            printf("Subscriber received: %s", msg);
            sem_post(sem);
            if(msg[0] == '\n')
                break;
        }
        close(pipefd[0]);
        return 0;
    } else {  // Parent process (publisher)
        close(pipefd[0]);  // Close unused read end
        printf("Publisher: ");
        while (fgets(msg, MSG_SIZE, stdin) != NULL) {
            write(pipefd[1], msg, MSG_SIZE);
            sem_wait(sem);
            if(msg[0] == '\n')
                break;
            printf("Publisher: ");
        }
        close(pipefd[1]);
        wait(NULL);  // Wait for child to die >:)

        sem_destroy(sem);
        munmap(sem, sizeof(sem_t));

        return 0;
    }

    return 0;
}
