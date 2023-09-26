#include <stdio.h>
#include <stdlib.h>
#include <limits.h>
#include <float.h>

// Function declarations for aggregation operations
void* add(const void* a, const void* b);
void* multiply(const void* a, const void* b);
void* max(const void* a, const void* b);

// Aggregation function
void* aggregate(void* base, size_t size, int n, void* initial_value, void* (*opr)(const void*, const void*)) {
    char* ptr = (char*) base;
    for(int i = 0; i < n; i++, ptr += size)
        initial_value = opr(initial_value, ptr);
    return initial_value;
}

int main() {
    // Define arrays of integers and doubles
    int intArr[] = {1, 2, 3, 4, 5};
    double doubleArr[] = {1.1, 2.2, 3.3, 4.4, 5.5};

    // Initial values for operations
    int intZero = 0, intOne = 1, intMin = INT_MIN;
    double doubleZero = 0, doubleOne = 1, doubleMin = -DBL_MAX;

    // Perform aggregate operations on integer array
    int intSum = *(int*)aggregate(intArr, sizeof(int), 5, &intZero, add);
    int intProduct = *(int*)aggregate(intArr, sizeof(int), 5, &intOne, multiply);
    int intMax = *(int*)aggregate(intArr, sizeof(int), 5, &intMin, max);

    // Perform aggregate operations on double array
    double doubleSum = *(double*)aggregate(doubleArr, sizeof(double), 5, &doubleZero, add);
    double doubleProduct = *(double*)aggregate(doubleArr, sizeof(double), 5, &doubleOne, multiply);
    double doubleMax = *(double*)aggregate(doubleArr, sizeof(double), 5, &doubleMin, max);

    // Print the results
    printf("Integer Array Results\n");
    printf("Sum: %d\n", intSum);
    printf("Product: %d\n", intProduct);
    printf("Max: %d\n", intMax);

    printf("\nDouble Array Results\n");
    printf("Sum: %f\n", doubleSum);
    printf("Product: %f\n", doubleProduct);
    printf("Max: %f\n", doubleMax);

    return 0;
}

// Function definitions for aggregation operations

void* add(const void* a, const void* b) {
    if(sizeof(*a) == sizeof(int)) {
        int* result = (int*)malloc(sizeof(int));
        *result = *(int*)a + *(int*)b;
        return result;
    } else if(sizeof(*a) == sizeof(double)) {
        double* result = (double*)malloc(sizeof(double));
        *result = *(double*)a + *(double*)b;
        return result;
    } else {
        fprintf(stderr, "Unsupported data type in add operation\n");
        exit(EXIT_FAILURE);
    }
}

void* multiply(const void* a, const void* b) {
    if(sizeof(*a) == sizeof(int)) {
        int* result = (int*)malloc(sizeof(int));
        *result = *(int*)a * *(int*)b;
        return result;
    } else if(sizeof(*a) == sizeof(double)) {
        double* result = (double*)malloc(sizeof(double));
        *result = *(double*)a * *(double*)b;
        return result;
    } else {
        fprintf(stderr, "Unsupported data type in multiply operation\n");
        exit(EXIT_FAILURE);
    }
}

void* max(const void* a, const void* b) {
    if(sizeof(*a) == sizeof(int)) {
        int* result = (int*)malloc(sizeof(int));
        *result = *(int*)a > *(int*)b ? *(int*)a : *(int*)b;
        return result;
    } else if(sizeof(*a) == sizeof(double)) {
        double* result = (double*)malloc(sizeof(double));
        *result = *(double*)a > *(double*)b ? *(double*)a : *(double*)b;
        return result;
    } else {
        fprintf(stderr, "Unsupported data type in max operation\n");
        exit(EXIT_FAILURE);
    }
}
