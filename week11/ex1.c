// ex1
#include <stdio.h>
#include <dirent.h>

int main() {
    DIR *dir;
    struct dirent *entry;

    // Open the root directory
    dir = opendir("/");
    if (dir == NULL) {
        perror("opendir");
        return 1;
    }

    // Read and print each directory entry
    while ((entry = readdir(dir)) != NULL) {
        printf("%s\n", entry->d_name);
    }

    // Close the directory
    closedir(dir);
    return 0;
}

