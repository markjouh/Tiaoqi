#pragma once

#include <array>
#include <queue>

auto __get_dists(int u) {
    std::array<char, SPACES> dist;
    std::fill(begin(dist), end(dist), -1);
    dist[u] = 0;
    
    std::queue<int> q;
    q.push(u);

    while (!q.empty()) {
        int u = q.front();
        q.pop();
        for (int i = 0; i < 6; i++) {
            const int v = graph[u][i];
            if (v != -1 && dist[v] == -1) {
                dist[v] = dist[u] + 1;
                q.push(v);
            }
        }
    }
    
    return dist;
}