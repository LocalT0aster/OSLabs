#include <stdio.h>
#include <string.h>

#define MAX_FILENAME_LENGTH 63
#define MAX_PATH_SIZE 2048
#define MAX_FILE_DATA_SIZE 1024

// File structure definition
typedef struct File {
    unsigned int id;
    char name[MAX_FILENAME_LENGTH + 1];
    size_t size;
    char data[MAX_FILE_DATA_SIZE];
    struct Directory* dir;
} File;

// Directory structure definition
typedef struct Directory {
    char name[MAX_FILENAME_LENGTH + 1];
    File files[255]; // Max 255 files as we're using unsigned char for nf
    struct Directory* dirs[255]; // Max 255 directories as we're using unsigned char for nd
    unsigned char nf;
    unsigned char nd;
    char path[MAX_PATH_SIZE];
} Directory;

// File operations
void overwrite_to_file(File* file, const char* str) {
    strncpy(file->data, str, MAX_FILE_DATA_SIZE - 1);
    file->size = strlen(file->data) + 1;
}

void append_to_file(File* file, const char* str) {
    strncat(file->data, str, MAX_FILE_DATA_SIZE - file->size);
    file->size = strlen(file->data) + 1;
}

void printp_file(File* file) {
    printf("%s/%s\n", file->dir->path, file->name);
}

// Directory operations
void add_file(File* file, Directory* dir) {
    if (dir->nf < 255) {
        dir->files[dir->nf] = *file;
        file->dir = dir;
        dir->nf++;
    } else {
        printf("Directory full! Cannot add more files.\n");
    }
}

int main() {
    // Create the root directory
    Directory root = {"", {}, {}, 0, 0, "/"};

    // Create two subdirectories home and bin
    Directory home = {"home", {}, {}, 0, 0, "/home"};
    Directory bin = {"bin", {}, {}, 0, 0, "/bin"};

    root.dirs[root.nd++] = &home;
    root.dirs[root.nd++] = &bin;

    // Add a file bash to the directory bin
    File bash = {1, "bash", 0, "", &bin};
    add_file(&bash, &bin);

    // Add two files ex3_1.c and ex3_2.c to the directory home
    File ex3_1 = {2, "ex3_1.c", 0, "", &home};
    File ex3_2 = {3, "ex3_2.c", 0, "", &home};
    add_file(&ex3_1, &home);
    add_file(&ex3_2, &home);

    // Add the content to the files
    overwrite_to_file(&ex3_1, "int printf(const char * format, ...);");
    overwrite_to_file(&ex3_2, "//This is a comment in C language");
    overwrite_to_file(&bash, "Bourne Again Shell!!");
    append_to_file(&ex3_1, "int main(){printf(\"Hello World!\");}");

    // Print the path of all files in the system by calling the function printp_file
    printp_file(&bash);
    printp_file(&ex3_1);
    printp_file(&ex3_2);
    return 0;
}
