#include <stdio.h>
#include <string.h>
#include <stdbool.h>
#include <math.h>

bool isValid(long long number, int base) {
    while (number) {
        if (number % 10 >= base) {
            return false;
        }
        number /= 10;
    }
    return true;
}

long long toDecimal(long long number, int base) {
    long long decimal = 0;
    int i = 0;
    while (number) {
        decimal += (number % 10) * pow(base, i);
        number /= 10;
        i++;
    }
    return decimal;
}

void fromDecimal(long long decimal, int base, char* result) {
    char buffer[100];
    int i = 0;
    while (decimal) {
        buffer[i] = '0' + (decimal % base);
        decimal /= base;
        i++;
    }
    buffer[i] = '\0';

    // Reverse the buffer to get the final result
    for (int j = 0; j < i; j++) {
        result[j] = buffer[i - j - 1];
    }
    result[i] = '\0';
}

void convert(long long number, int s, int t) {
    if (s < 2 || s > 10 || t < 2 || t > 10 || !isValid(number, s)) {
        printf("cannot convert!\n");
        return;
    }

    long long decimal = toDecimal(number, s);
    char result[100];
    fromDecimal(decimal, t, result);
    printf("%s\n", result);
}

int main() {
    long long number;
    int s, t;
    printf("Enter the number: ");
    if(scanf("%lld", &number) != 1) {
        printf("Invalid input for number!\n");
        return 1;
    }
    printf("Enter the source numeral system (between 2 and 10): ");
    if(scanf("%d", &s) != 1) {
        printf("Invalid input for source numeral system!\n");
        return 1;
    }
    printf("Enter the target numeral system (between 2 and 10): ");
    if(scanf("%d", &t) != 1) {
        printf("Invalid input for target numeral system!\n");
        return 1;
    }
    convert(number, s, t);
    return 0;
}
