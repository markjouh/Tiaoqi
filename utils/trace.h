#pragma once

#include <stdio.h>
#include <stdarg.h>

void trace_array(const int *arr, int n) {
    printf("{");
    for (int i = 0; i < n; i++) {
        if (i > 0) {
            printf(", ");
        }
        printf("%d", arr[i]);
    }
    printf("}");
}

void trace_array2d(const int *arr, int n, int m) {
    printf("{\n");
    for (int i = 0; i < n; i++) {
        if (i > 0) {
            printf(",\n");
        }
        trace_array(arr + i * m, m);
    }
    printf("\n}");
}