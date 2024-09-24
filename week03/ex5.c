#include <stdio.h>
#include <stdlib.h>
#include <stdarg.h>
#include <limits.h>
#include <float.h>
#include <errno.h>

typedef struct StackNode StackNode;

struct StackNode {
    void* p;
    StackNode* prev;
};
StackNode* top = NULL;

void push(void* p) {
    StackNode* temp = calloc(1, sizeof(StackNode));
    temp->p = p;
    temp->prev = top;
    top = temp;
}

StackNode* pop() {
    if (top == NULL) {
        return NULL;
    }
    StackNode* temp = top;
    top = top->prev;
    return temp;
}

// Function declarations for aggregation operations
void* add_int(const void* a, const void* b);
void* multiply_int(const void* a, const void* b);
void* max_int(const void* a, const void* b);
void* add_double(const void* a, const void* b);
void* multiply_double(const void* a, const void* b);
void* max_double(const void* a, const void* b);

// Aggregation function
void* vaggregate(void* base, size_t size, int n, void* initial_value, void* (*opr)(const void*, const void*), ...) {
    char* ptr = (char*)base;
    for (int i = 0; i < n; i++, ptr += size)
        initial_value = opr(initial_value, ptr);
    return initial_value;
}

int main() {
    // Define arrays of integers and doubles
    int intArr[] = { 1, 2, 3, 4, 5 };
    double doubleArr[] = { 1.1, 2.2, 3.3, 4.4, 5.5 };

    // Initial values for operations
    int intZero = 0, intOne = 1, intMin = INT_MIN;
    double doubleZero = 0, doubleOne = 1, doubleMin = -DBL_MAX;

    // Perform aggregate operations on integer array
    int intSum = *(int*)vaggregate(intArr, sizeof(int), 5, &intZero, add_int);
    int intProduct = *(int*)vaggregate(intArr, sizeof(int), 5, &intOne, multiply_int);
    int intMax = *(int*)vaggregate(intArr, sizeof(int), 5, &intMin, max_int);

    // Perform aggregate operations on double array
    double doubleSum = *(double*)vaggregate(doubleArr, sizeof(double), 5, &doubleZero, add_double);
    double doubleProduct = *(double*)vaggregate(doubleArr, sizeof(double), 5, &doubleOne, multiply_double);
    double doubleMax = *(double*)vaggregate(doubleArr, sizeof(double), 5, &doubleMin, max_double);

    // Print the results
    printf("Integer Array Results\n");
    printf("Sum: %d\n", intSum);
    printf("Product: %d\n", intProduct);
    printf("Max: %d\n", intMax);

    printf("\nDouble Array Results\n");
    printf("Sum: %f\n", doubleSum);
    printf("Product: %f\n", doubleProduct);
    printf("Max: %f\n", doubleMax);

    StackNode* temp;
    while ((temp = pop()) != NULL) {
        free(temp);
    }
    return 0;
}

// Function definitions for aggregation operations

void* add_int(const void* a, const void* b) {
    int* result = (int*)malloc(sizeof(int));
    push(result);
    *result = *(int*)a + *(int*)b;
    return result;
}

void* add_double(const void* a, const void* b) {
    double* result = (double*)malloc(sizeof(double));
    push(result);
    *result = *(double*)a + *(double*)b;
    return result;
}

void* multiply_int(const void* a, const void* b) {
    int* result = (int*)malloc(sizeof(int));
    push(result);
    *result = *(int*)a * *(int*)b;
    return result;
}

void* multiply_double(const void* a, const void* b) {
    double* result = (double*)malloc(sizeof(double));
    push(result);
    *result = *(double*)a * *(double*)b;
    return result;
}

void* max_int(const void* a, const void* b) {
    int* result = (int*)malloc(sizeof(int));
    push(result);
    *result = *(int*)a > *(int*)b ? *(int*)a : *(int*)b;
    return result;
}

void* max_double(const void* a, const void* b) {
    double* result = (double*)malloc(sizeof(double));
    push(result);
    *result = *(double*)a > *(double*)b ? *(double*)a : *(double*)b;
    return result;
}
