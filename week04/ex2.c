#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <time.h>

#define SIZE 120


void generate_vector(int* p) {
    for (int i = 0; i < SIZE; i++) {
        p[i] = rand() % 100;
    }
}

int dot_product(int* a, int* b, int size) {
    int sum = 0;
    for (int i = 0; i < size; ++i) {
        sum += a[i] * b[i];
    }
    return sum;
}

int main() {
    srand(time(NULL));
    int u[SIZE], v[SIZE];
    generate_vector(u);
    generate_vector(v);

    int n;
    printf("Enter the number of processes: ");
    scanf("%d", &n);
    n %= 121;

    int el_per_process = SIZE / n;
    char isEvenLoaded = el_per_process * n == SIZE;

    FILE* file = fopen("temp.txt", "w");
    if (file == NULL) {
        perror("Failed to open the file");
        exit(1);
    }
    int s, i, start;
    pid_t pid;
    if (n < 1) {
        dot_product(u, v, SIZE);
    } else {
        for (i = 0; i < n; ++i) {
            pid = fork();

            if (pid == 0) {  // Child process
                start = i * el_per_process;
                s = !isEvenLoaded && i == n - 1 ? SIZE + el_per_process * (1 - n) : el_per_process;
                fprintf(file, "%d\n", dot_product(u + start, v + start, s));
                fclose(file);
                exit(0);  // Important: make sure child process exits after computation
            } else if (pid < 0) {
                perror("Fork failed");
                exit(1);
            }
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
