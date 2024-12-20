#pragma once

#include "../utils/queue.h"

int bufDist[numSpaces];

void calcDists(int u) {
    memset(bufDist, -1, sizeof bufDist);
    clearQueue();

    bufDist[u] = 0;
    pushBack(u);

    while (!queueEmpty()) {
        int u = queuePop();
        for (int i = 0; i < 6; i++) {
            const int v = graph[u][i];
            if (v != -1 && bufDist[v] == -1) {
                bufDist[v] = bufDist[u] + 1;
                pushBack(v);
            }
        }
    }
}

void getDists(int u, int *arr) {
    calcDists(u);
    memcpy(arr, bufDist, sizeof bufDist);
}