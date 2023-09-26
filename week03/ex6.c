#include<stdio.h>
#define T int

void swap(T* a, T* b) {
    T temp = *a;
    *a = *b;
    *b = temp;
}

void quicksort(T* arr, size_t size) {
    if (size <= 1) return;
    if (size == 2) {
        if (arr[0] > arr[1]) {
            swap(&arr[0], &arr[1]);
            return;
        }
    }
    T piv = size>>1; // Place pivot in the middle.
    int l = piv - 1;
    int r = piv;
    piv = arr[piv];
}

int main() {
    
    return 0;
}
