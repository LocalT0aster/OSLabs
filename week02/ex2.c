#include <stdio.h>
#include <string.h>

int main() {
    char str[256];
    int index = 0;
    char ch;

    // Read characters until dot (.) or Enter key is pressed
    while (1) {
        ch = getchar();

        if (ch == '.' || ch == '\n') {
            break;
        }

        if (index < 255) {  // To avoid exceeding the buffer size
            str[index++] = ch;
        }
    }

    str[index] = '\0';  // Null-terminate the string

    printf("\"");
    for (int i = strlen(str) - 1; i >= 0; i--) {
        putchar(str[i]);
    }
    printf("\"\n");

    return 0;
}
