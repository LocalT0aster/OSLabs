#include<stdio.h>
#include<stdlib.h>
int temp;

int const_tri(int* p, int n) {
    if (n < 1) return 0;
    if (n < 3) return 1;
    if (n == 3) return 2;
    return const_tri(p, n - 3) + const_tri(p, n - 2) + const_tri(p, n - 1);
}

int main() {
    // Create x and q
    const int x = 1;
    const int* q = &x;
    // Initialize p
    int* p = calloc(3, sizeof(int));
    // Assign the value x and print addresses of cells of p using temp in for loop
    for (temp = 0; temp < 3; temp++) {
        p[temp] = *q;
        printf("%p\n", &p[temp]);
    }
    p[2] *= 2; // multiply x by 2
    // Check if cells are contiguous
    if ((char*)&p[2] - (char*)&p[1] != sizeof(int) || (char*)&p[1] - (char*)&p[0] != sizeof(int))
        printf("Cells are not contiguous!, difference:%ld, %ld\n", (char*)&p[2] - (char*)&p[1], (char*)&p[1] - (char*)&p[0]);
    else
        printf("Cells are contiguous.\n");
    printf("%d\n%d\n", const_tri(p, 4), const_tri(p, 36));
    free(p);
    return 0;
}
