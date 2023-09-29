#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <unistd.h>

#define MAX_INPUT_SIZE 1024
#define MAX_ARG_SIZE 100

int main() {
    char input[MAX_INPUT_SIZE];
    char* args[MAX_ARG_SIZE];
    
    while (1) {
        // Print shell prompt
        printf("my_shell> ");
        fflush(stdout);

        // Read user input
        if (fgets(input, sizeof(input), stdin) == NULL) {
            perror("fgets failed");
            exit(EXIT_FAILURE);
        }

        // Remove trailing newline character from the input
        size_t len = strlen(input);
        if (len > 0 && input[len - 1] == '\n') {
            input[len - 1] = '\0';
        }

        // If user enters "exit", exit the shell
        if (strcmp(input, "exit") == 0) {
            exit(EXIT_SUCCESS);
        }

        // Tokenize the input string into arguments
        int arg_count = 0;
        char* token = strtok(input, " ");
        while (token != NULL) {
            args[arg_count++] = token;
            if (arg_count >= MAX_ARG_SIZE - 1) break;
            token = strtok(NULL, " ");
        }
        args[arg_count] = NULL;  // Last element of args should be NULL
        
        if (arg_count == 0) continue; // Empty input, continue to the next iteration

        // Create a child process to execute the command
        pid_t pid = fork();
        if (pid < 0) {
            perror("fork failed");
            exit(EXIT_FAILURE);
        } else if (pid == 0) {
            // Child process: Execute the command
            if (execvp(args[0], args) < 0) {
                perror("execvp failed");
                exit(EXIT_FAILURE);
            }
        } else {
            // Parent process: Wait for the child to terminate
            if (waitpid(pid, NULL, 0) < 0) {
                perror("waitpid failed");
                exit(EXIT_FAILURE);
            }
        }
    }

    return EXIT_SUCCESS;
}
