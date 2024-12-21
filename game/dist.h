#pragma once

#include "../utils/queue.h"

int dist_buf[SPACES];

void dist_calc(int u) {
    memset(dist_buf, -1, sizeof dist_buf);
    queue_clear();

    dist_buf[u] = 0;
    queue_pb(u);

    while (!queue_empty()) {
        int u = queue_pop();
        for (int i = 0; i < 6; i++) {
            const int v = graph[u][i];
            if (v != -1 && dist_buf[v] == -1) {
                dist_buf[v] = dist_buf[u] + 1;
                queue_pb(v);
            }
        }
    }
}

void dist_copy(int u, int *arr) {
    dist_calc(u);
    memcpy(arr, dist_buf, sizeof dist_buf);
}