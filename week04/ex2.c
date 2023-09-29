#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/wait.h>
#include<time.h>

#define SIZE 120

int u[SIZE], v[SIZE];

void generate_vectors() {
    for (int i = 0; i < SIZE; i++) {
        u[i] = rand() % 100;
        v[i] = rand() % 100;
    }
}

int dot_product(int start, int end) {
    int sum = 0;
    for (int i = start; i < end; i++) {
        sum += u[i] * v[i];
    }
    return sum;
}

int main() {
    srand(time(NULL));
    generate_vectors();

    int n;
    printf("Enter the number of processes: ");
    scanf("%d", &n);

    int elements_per_process = SIZE / n;

    FILE *file = fopen("temp.txt", "w");
    if (file == NULL) {
        perror("Failed to open the file");
        exit(1);
    }

    for (int i = 0; i < n; i++) {
        pid_t pid = fork();

        if (pid == 0) {  // Child process
            int start = i * elements_per_process;
            int end = start + elements_per_process;
            int result = dot_product(start, end);

            fprintf(file, "%d\n", result);
            fclose(file);
            exit(0);  // Important: make sure child process exits after computation
        } else if (pid < 0) {
            perror("Fork failed");
            exit(1);
        }
    }

    for (int i = 0; i < n; i++) {
        wait(NULL);
    }

    fclose(file);

    file = fopen("temp.txt", "r");
    int sum = 0;
    int temp;
    while (fscanf(file, "%d", &temp) == 1) {
        sum += temp;
    }
    fclose(file);

    printf("Dot product: %d\n", sum);

    return 0;
}
