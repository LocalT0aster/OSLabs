#ifdef ASSIGNMENT_NOT_DONE
// ex4
#define _GNU_SOURCE
// #define FILE_OUTPUT

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <signal.h>
#include <string.h>
#include <sys/wait.h>
#include <sys/time.h>
#define PS_MAX 10

int console_stdout; // holds console stdout

// holds the scheduling data of one process
typedef struct {
    int idx; // process idx (index)
    int at; // arrival time
    int bt; // burst time
    int rt; // response time
    int wt; // waiting time
    int ct; // completion time
    int tat; // turnaround time
    int burst; // remaining burst
} ProcessData;

// the idx of the running process
int running_process = -1; // -1 means no running processes

// the total time of the timer
int total_time; // should increment one second at a time by the scheduler

// data of the processes
ProcessData data[PS_MAX]; // array of process data

// array of all process pids
pid_t ps[PS_MAX]; // zero valued pids - means the process is terminated or not created yet

// size of data array
int data_size;

void read_file(FILE* file) {
    int idx, at, bt;
    data_size = 0;

    // Skip header
    char line[100];  // assuming one line won't be longer than 100 characters
    fgets(line, sizeof(line), file);  // reads the first line, effectively skipping the header

    // Read the data
    while (fscanf(file, "%d %d %d", &idx, &at, &bt) == 3 && data_size < PS_MAX) {
        data[data_size].idx = idx;
        data[data_size].at = at;
        data[data_size].bt = bt;
        data[data_size].burst = bt;
        ps[data_size] = 0; // initialize with 0
        data_size++;
    }

    fclose(file);
}

// send signal SIGCONT to the worker process
void resume(pid_t process) {
    if (process > 0) {
        kill(process, SIGCONT);
    }
}

// send signal SIGTSTP to the worker process
void suspend(pid_t process) {
    if (process > 0) {
        kill(process, SIGTSTP);
    }
}

// send signal SIGTERM to the worker process
void terminate(pid_t process) {
    if (process > 0) {
        kill(process, SIGTERM);
        waitpid(process, NULL, 0); // wait for the child to terminate
    }
}

// create a process using fork
void create_process(int new_process) {
    // stop the running process
    if (running_process != -1) {
        suspend(ps[running_process]);
    }

    // fork a new process
    pid_t pid = fork();
    if (pid == 0) { // Child process
        char idx_arg[10];
        sprintf(idx_arg, "%d", new_process);
        char* args[] = { "./worker.exe", idx_arg, NULL };
        execvp(args[0], args);
        exit(0); // exit if execvp fails
    } else if (pid > 0) {
        ps[new_process] = pid; // save the PID of the child process
    } else {
        perror("Fork failed");
        exit(1);
    }

    // Now the idx of the running process is new_process
    running_process = new_process;
}


// find next process for running
ProcessData find_next_process() {
    int location = -1;

    for (int i = 0; i < data_size; i++) {
        if (data[i].burst > 0 && (location == -1 || data[i].at < data[location].at || (data[i].at == data[location].at && data[i].idx < data[location].idx))) {
            location = i;
        }
    }

    // if next_process did not arrive so far,
    // then we recursively call this function after incrementing total_time
    if (data[location].at > total_time) {
        printf("Scheduler: Runtime: %u seconds.\nProcess %d: has not arrived yet.\n", total_time, location);
        fflush(stdout);
        total_time++;
        return find_next_process();
    }

    // return the data of next process
    return data[location];
}


// reports the metrics and simulation results
void report() {
    printf("Simulation results.....\n");
    int sum_wt = 0;
    int sum_tat = 0;
    for (int i = 0; i < data_size; i++) {
        printf("process %d:\n", i);
        printf("\tat=%d\n", data[i].at);
        printf("\tbt=%d\n", data[i].bt);
        printf("\tct=%d\n", data[i].ct);
        printf("\twt=%d\n", data[i].wt);
        printf("\ttat=%d\n", data[i].tat);
        printf("\trt=%d\n", data[i].rt);
        sum_wt += data[i].wt;
        sum_tat += data[i].tat;
    }

    printf("data size = %d\n", data_size);
    float avg_wt = (float)sum_wt / data_size;
    float avg_tat = (float)sum_tat / data_size;
    printf("Average results for this run:\n");
    printf("\tavg_wt=%f\n", avg_wt);
    printf("\tavg_tat=%f\n", avg_tat);
    fflush(stdout);
}

void check_burst() {
    for (int i = 0; i < data_size; i++)
        if (data[i].burst > 0)
            return;

    // report simulation results
    report();

#ifdef FILE_OUTPUT
    fflush(stdout);
    fclose(stdout);
    if (dup2(console_stdout, STDOUT_FILENO) == -1) {
        perror("Failed to restore stdout");
        exit(1);
    }
    close(console_stdout);
#endif

    // kill the scheduler >:)
    exit(0);
}


// This function is called every one second as handler for SIGALRM signal
void schedule_handler(int signum) {
    // increment the total time
    total_time++;
    // 1. If there is a worker process running
    if (running_process != -1) {
        // decrement its remaining burst
        data[running_process].burst--;

        printf("Scheduler: Runtime: %u seconds\n", total_time);
        printf("Process %d is running with %d seconds left\n", running_process, data[running_process].burst);
        fflush(stdout);

        // 1.A. If the worker process finished its burst time
        if (data[running_process].burst <= 0) {
            printf("Scheduler: Terminating Process %d (Remaining Time: %d)\n", running_process, data[running_process].burst);
            fflush(stdout);

            // terminate the running process
            terminate(ps[running_process]);

            // wait for its termination
            waitpid(ps[running_process], NULL, 0);

            // Calculate its metrics {ct, tat, wt}
            data[running_process].ct = total_time;
            data[running_process].tat = data[running_process].ct - data[running_process].at;
            data[running_process].wt = data[running_process].tat - data[running_process].bt;
            running_process = -1;
        }
    }

    // 2. After that, we need to find the next process to run
    ProcessData next_process_data = find_next_process();
    int next_process = next_process_data.idx;

    // check the bursts of all processes
    check_burst();

    // 3. If {next_process} is not running
    if (next_process != running_process) {
        // 3.A. If the current process is running
        if (running_process != -1) {
            printf("Scheduler: Stopping Process %d (Remaining Time: %d)\n", running_process, data[running_process].burst);
            fflush(stdout);

            // stop the current running process
            suspend(ps[running_process]);
        }

        // 3.B. set current process {next_process} as the running process.
        running_process = next_process;

        // if the process was never started (its pid is zero)
        if (ps[running_process] == 0) {
            // 3.C.1. then create a new process for {running_process}
            printf("Scheduler: Starting Process %d (Remaining Time: %d)\n", running_process, data[running_process].burst);
            fflush(stdout);

            // Here we have the first response to the process {running_process}
            data[running_process].rt = total_time - data[running_process].at;

            create_process(running_process);
        } else {
            // 3.C.2. or resume the process {running_process}
            printf("Scheduler: Resuming Process %d (Remaining Time: %d)\n", running_process, data[running_process].burst);
            fflush(stdout);

            resume(ps[running_process]);
        }
    }
}

int main(int argc, char* argv[]) {
#ifdef FILE_OUTPUT
    console_stdout = dup(STDOUT_FILENO);
    if (console_stdout == -1) {
        perror("Failed to duplicate stdout");
        return 1;
    }
    FILE* fp = freopen("scheduler.log", "a", stdout);
    if (!fp) {
        perror("Failed to redirect stdout to file");
        return 1;
    }
#endif

    // read the data file
    FILE* in_file = fopen(argv[1], "r");
    if (in_file == NULL) {
        perror("File is not found or cannot open it!\n");
        exit(1);
    } else {
        read_file(in_file);
    }

    // set a timer
    struct itimerval timer;

    // the timer goes off 1 second after reset
    timer.it_value.tv_sec = 1;
    timer.it_value.tv_usec = 0;

    // timer increments 1 second at a time
    timer.it_interval.tv_sec = 1;
    timer.it_interval.tv_usec = 0;

    // this counts down and sends SIGALRM to the scheduler process after expiration.
    setitimer(ITIMER_REAL, &timer, NULL);

    // register the handler for SIGALRM signal
    signal(SIGALRM, schedule_handler);

    // Wait till all processes finish
    while (1); // infinite loop
}
#endif