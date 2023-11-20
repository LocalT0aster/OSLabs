// ex1
#define _GNU_SOURCE

#include <stdio.h>
#include <stdlib.h>
#include <sys/inotify.h>
#include <unistd.h>
#include <signal.h>
#include <sys/stat.h>
#include <dirent.h>
#include <string.h>

#define EVENT_SIZE (sizeof (struct inotify_event))
#define EVENT_BUF_LEN (1024 * (EVENT_SIZE + 16))

int inotifyFd;
char *monitored_path; // Global variable for the monitored directory path

void print_directory_stat(char *path) {
    DIR *dir;
    struct dirent *entry;
    struct stat file_stat;
    char full_path[1024];

    if ((dir = opendir(path)) == NULL) {
        perror("Unable to open directory");
        return;
    }

    while ((entry = readdir(dir)) != NULL) {
        if (strcmp(entry->d_name, ".") == 0 || strcmp(entry->d_name, "..") == 0)
            continue;

        snprintf(full_path, sizeof(full_path), "%s/%s", path, entry->d_name);
        if (stat(full_path, &file_stat) == 0) {
            printf("File: %s, Inode: %ld, Size: %ld bytes\n",
                   full_path, file_stat.st_ino, file_stat.st_size);
        } else {
            perror("stat");
        }
    }

    closedir(dir);
}

void signal_handler(int sig) {
    if (sig == SIGTERM) {
        print_directory_stat(monitored_path);
        exit(0);
    }
}

int main(int argc, char* argv[]) {
    if (argc < 2) {
        fprintf(stderr, "Usage: %s <path_to_watch>\n", argv[0]);
        exit(EXIT_FAILURE);
    }

    char* monitored_path = argv[1];
    char buffer[EVENT_BUF_LEN];
    int length, i = 0;

    // Initialize inotify
    inotifyFd = inotify_init();
    if (inotifyFd < 0) {
        perror("inotify_init");
    }

    // Add directory to be monitored
    int watchDescriptor = inotify_add_watch(inotifyFd, monitored_path, IN_MODIFY | IN_CREATE | IN_DELETE);
    if (watchDescriptor == -1) {
        perror("inotify_add_watch");
        exit(EXIT_FAILURE);
    }

    // Signal handling setup for SIGTERM
    signal(SIGTERM, signal_handler);

    // Main loop to read events
    while (1) {
        int i = 0;
        int length = read(inotifyFd, buffer, EVENT_BUF_LEN);
        if (length < 0) {
            perror("read");
        }

        while (i < length) {
            struct inotify_event* event = (struct inotify_event*)&buffer[i];
            if (event->len) {
                if (event->mask & IN_CREATE) {
                    printf("New file %s created.\n", event->name);
                }
                if (event->mask & IN_DELETE) {
                    printf("File %s deleted.\n", event->name);
                }
                if (event->mask & IN_MODIFY) {
                    printf("File %s modified.\n", event->name);
                }
            }
            i += EVENT_SIZE + event->len;
        }
    }

    // Cleanup
    inotify_rm_watch(inotifyFd, watchDescriptor);
    close(inotifyFd);

    return 0;
}
