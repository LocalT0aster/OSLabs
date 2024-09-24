#include <ctype.h>
#include <stdio.h>
#include <string.h>
#include <errno.h>

char *dict; // Memory greedy memoization approach

// Function to count the occurrence of a character in a string
int count(const char* str, char c) {
    if (dict[c] != 0) {
        return dict[c];
    }
    int cnt = 0;
    for (int i = 0; str[i] != '\0'; ++i) {
        if (tolower(str[i]) == tolower(c)) {
            ++cnt;
        }
    }
    dict[c] = cnt;
    return cnt;
}

// Function to count and print the occurrences of all characters in a string
void countAll(const char* str) {
    for (int i = 0; str[i] != '\0'; ++i) {
        if (i != 0) printf(", ");
        printf("%c:%d", tolower(str[i]), count(str, str[i]));
    }
    printf("\n");
}

int main(int argc, char* argv[]) {
    char inputStr[256]; // To store the string if not provided as an argument
    dict = calloc(256, 1);

    if (argc == 2) {
        // Count all character occurrences using the argument
        countAll(argv[1]);
    } else {
        printf("Enter a string: ");
        if (fgets(inputStr, sizeof(inputStr), stdin) == NULL) {
            errno = EIO;
            perror("Error reading input");
            free(dict);
            return EIO;
        }

        // Remove trailing newline character
        int len = strlen(inputStr);
        if (len > 0 && inputStr[len - 1] == '\n') {
            inputStr[len - 1] = '\0';
        }

        // Count all character occurrences using the entered string
        countAll(inputStr);
    }
    free(dict);

    return 0;
}
