// ex1
#define _GNU_SOURCE

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <dirent.h>
#include <string.h>

void find_all_hlinks(const char* source) {
    struct stat source_stat;
    if (stat(source, &source_stat) != 0) {
        perror("Error getting source file stats");
        return;
    }

    DIR* dir;
    struct dirent* entry;
    struct stat file_stat;
    const char* dir_path = "."; // Assuming current directory, can be changed

    if ((dir = opendir(dir_path)) == NULL) {
        perror("Unable to open directory");
        return;
    }

    while ((entry = readdir(dir)) != NULL) {
        if (strcmp(entry->d_name, ".") == 0 || strcmp(entry->d_name, "..") == 0)
            continue;

        if (stat(entry->d_name, &file_stat) != 0) {
            perror("Error getting file stats");
            continue;
        }

        if (file_stat.st_ino == source_stat.st_ino) {
            printf("Hard link found: %s\n", entry->d_name);
        }
    }

    closedir(dir);
}

void unlink_all(const char* source) {
    struct stat source_stat;
    if (stat(source, &source_stat) != 0) {
        perror("Error getting source file stats");
        return;
    }

    DIR* dir;
    struct dirent* entry;
    struct stat file_stat;
    const char* dir_path = ".";
    int found = 0;

    if ((dir = opendir(dir_path)) == NULL) {
        perror("Unable to open directory");
        return;
    }

    while ((entry = readdir(dir)) != NULL) {
        if (strcmp(entry->d_name, ".") == 0 || strcmp(entry->d_name, "..") == 0)
            continue;

        if (stat(entry->d_name, &file_stat) != 0) {
            perror("Error getting file stats");
            continue;
        }

        if (file_stat.st_ino == source_stat.st_ino && strcmp(entry->d_name, source) != 0) {
            if (!found) {
                printf("Keeping link: %s\n", entry->d_name);
                found = 1;
            } else {
                printf("Removing link: %s\n", entry->d_name);
                unlink(entry->d_name);
            }
        }
    }

    closedir(dir);
}

void create_sym_link(const char* source, const char* link) {
    if (symlink(source, link) != 0) {
        perror("Error creating symbolic link");
    } else {
        printf("Symbolic link created: %s -> %s\n", link, source);
    }
}

int main() {
    // Implement logic as per assignment instructions
    // Example usage of functions:
    create_sym_link("myfile1.txt", "myfile13.txt");
    find_all_hlinks("myfile1.txt");
    unlink_all("myfile11.txt");

    return 0;
}
