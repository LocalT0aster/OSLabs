#include<stdio.h>
#define T int

void swap(T* a, T* b) {
    T temp = *a;
    *a = *b;
    *b = temp;
}

int partition(T* arr, int low, int high) {
    T pivot = arr[high];
    int i = (low - 1); // Index of the smaller element

    for (int j = low; j <= high - 1; j++) {
        if (arr[j] < pivot) {
            i++; // Increment index of the smaller element
            swap(&arr[i], &arr[j]);
        }
    }
    swap(&arr[i + 1], &arr[high]);
    return (i + 1);
}

void quicksort_recursive(T* arr, int low, int high) {
    if (low < high) {
        // pi is partitioning index, arr[pi] is now at the right place
        int pi = partition(arr, low, high);

        // Separately sort elements before and after partition
        quicksort_recursive(arr, low, pi - 1);
        quicksort_recursive(arr, pi + 1, high);
    }
}

void quicksort(T* arr, size_t size) {
    if (size <= 1) return;
    if (size == 2) {
        if (arr[0] > arr[1]) {
            swap(&arr[0], &arr[1]);
            return;
        }
    }
    quicksort_recursive(arr, 0, size - 1);
}

int main() {
    T arr[] = {10, 7, 8, 9, 1, 5};
    int n = sizeof(arr) / sizeof(arr[0]);
    quicksort(arr, n);
    printf("Sorted array: \n");
    for (int i = 0; i < n; i++) {
        printf("%d ", arr[i]);
    }
    return 0;
}
