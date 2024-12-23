#pragma once

#include "board.h"

#include <array>
#include <queue>

auto __get_moves(Board board, int u) {
    const int old = board[u];
    board[u] = -1;
    
    std::array<int, SPACES> pred;
    std::fill(begin(pred), end(pred), -1);
    pred[u] = u;
    
    std::queue<int> q;
    q.push(u);
    
    while (!q.empty()) {
        int u = q.front();
        q.pop();
        
        for (int i = 0; i < 6; i++) {
            int v = u, dist = 0;

            while (board.free(v)) {
                v = graph[v][i];
                dist++;
            }

            if (v == -1) {
                continue;
            }

            bool ok = true;
            for (int j = 0; j < dist; j++) {
                v = graph[v][i];
                ok &= board.free(v);
            }

            if (ok && pred[v] == -1) {
                pred[v] = u;
                q.push(v);
            }
        }
    }

    for (int i = 0; i < 6; i++) {
        if (board.free(graph[u][i])) {
            pred[graph[u][i]] = u;
        }
    }

    pred[u] = -1;
    
    board[u] = old;
    
    return pred;
}