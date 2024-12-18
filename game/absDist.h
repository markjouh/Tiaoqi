#pragma once

#include "../utils/queue.h"

int absDist[numSpaces];

void findAbsDists(int u) {
    memset(absDist, -1, sizeof absDist);
    clearQueue();

    absDist[u] = 0;
    pushBack(u);

    while (!queueEmpty()) {
        int u = queuePop();
        for (int i = 0; i < 6; i++) {
            const int v = graph[u][i];
            if (v != -1 && absDist[v] == -1) {
                absDist[v] = absDist[u] + 1;
                pushBack(v);
            }
        }
    }
}