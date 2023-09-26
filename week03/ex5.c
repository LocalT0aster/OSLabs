#ifdef ASSIGNMENT_NOT_DONE
#include <stdio.h>
#include <stdlib.h>
#include <stdarg.h>
#include <limits.h>
#include <float.h>

// Modified Function declarations for aggregation operations to include type
void* add(const void* a, const void* b, char type);
void* multiply(const void* a, const void* b, char type);
void* max(const void* a, const void* b, char type);

// Variadic aggregate function
void* vaggregate(char type, int n, void* initial_value, void* (*opr)(const void*, const void*, char), ...) {
    va_list args;
    va_start(args, opr);
    
    for(int i = 0; i < n; i++) {
        void* elem = va_arg(args, void*);
        initial_value = opr(initial_value, &elem, type);
    }

    va_end(args);
    return initial_value;
}

int main() {
    // Initial values for operations
    int intZero = 0;
    double doubleZero = 0;
    
    // Perform variadic aggregate operations
    int intSum = *(int*)vaggregate('i', 5, &intZero, add, 1, 2, 3, 4, 5);
    double doubleSum = *(double*)vaggregate('d', 5, &doubleZero, add, 1.1, 2.2, 3.3, 4.4, 5.5);

    // Print the results
    printf("Integer Sum: %d\n", intSum);
    printf("Double Sum: %f\n", doubleSum);

    return 0;
}

// Modified aggregation operation functions to handle type
void* add(const void* a, const void* b, char type) {
    if(type == 'i') {
        int* result = (int*)malloc(sizeof(int));
        *result = *(int*)a + *(int*)b;
        return result;
    } else if(type == 'd') {
        double* result = (double*)malloc(sizeof(double));
        *result = *(double*)a + *(double*)b;
        return result;
    } else {
        fprintf(stderr, "Unsupported data type in add operation\n");
        exit(EXIT_FAILURE);
    }
}

void* multiply(const void* a, const void* b, char type) {
    if(type == 'i') {
        int* result = (int*)malloc(sizeof(int));
        *result = *(int*)a * *(int*)b;
        return result;
    } else if(type == 'd') {
        double* result = (double*)malloc(sizeof(double));
        *result = *(double*)a * *(double*)b;
        return result;
    } else {
        fprintf(stderr, "Unsupported data type in multiply operation\n");
        exit(EXIT_FAILURE);
    }
}

void* max(const void* a, const void* b, char type) {
    if(type == 'i') {
        int* result = (int*)malloc(sizeof(int));
        *result = *(int*)a > *(int*)b ? *(int*)a : *(int*)b;
        return result;
    } else if(type == 'd') {
        double* result = (double*)malloc(sizeof(double));
        *result = *(double*)a > *(double*)b ? *(double*)a : *(double*)b;
        return result;
    } else {
        fprintf(stderr, "Unsupported data type in max operation\n");
        exit(EXIT_FAILURE);
    }
}
#endif