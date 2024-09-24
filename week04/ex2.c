#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <time.h>
#include <errno.h>

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

    int p_num;
    printf("Enter the number of processes: ");
    scanf("%d", &p_num);
    p_num %= 121;
    if(p_num == 0){
        errno = 10;
        perror("No workers to do the job");
        exit(10);
    }

    int el_per_process = SIZE / p_num;
    char isEvenLoaded = el_per_process * p_num == SIZE;

    FILE* file = fopen("temp.txt", "w");
    if (file == NULL) {
        errno = 1;
        perror("Failed to open the file");
        exit(1);
    }
    int s, i, start;
    pid_t pid;
    if (p_num < 1) {
        dot_product(u, v, SIZE);
    } else {
        for (i = 0; i < p_num; ++i) {
            pid = fork();

            if (pid == 0) {  // Child process
                start = i * el_per_process;
                s = !isEvenLoaded && i == p_num - 1 ? SIZE + el_per_process * (1 - p_num) : el_per_process;
                fprintf(file, "%d\n", dot_product(u + start, v + start, s));
                fclose(file);
                exit(0);  // Important: make sure child process exits after computation
            } else if (pid < 0) {
                errno = 10;
                perror("Fork failed");
                exit(10);
            }
        }
    }

    for (int i = 0; i < p_num; i++) {
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
