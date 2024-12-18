#include <stdio.h>
#include <stdarg.h>

void printArray(const int *arr, int n) {
    printf("{");
    for (int i = 0; i < n; i++) {
        if (i > 0) {
            printf(", ");
        }
        printf("%d", arr[i]);
    }
    printf("}");
}

void printArray2d(const int *arr, int n, int m) {
    printf("{\n");
    for (int i = 0; i < n; i++) {
        if (i > 0) {
            printf(",\n");
        }
        printArray(arr + i * m, m);
    }
    printf("\n}");
}