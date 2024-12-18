bool reachable[numSpaces];

void dfsReachable(int u) {
    reachable[u] = true;

    for (int i = 0; i < 6; i++) {
        int v = u, dist = 0;

        while (checkFree(v)) {
            v = graph[v][i];
            dist++;
        }

        bool ok = true;
        for (int j = 0; j < dist; j++) {
            v = graph[v][i];
            ok &= checkFree(v);
        }

        if (ok && !reachable[v]) {
            dfsReachable(v);
        }
    }
}

void findReachable(int u) {
    const int origColor = board[u];
    board[u] = -1;

    // Multi-jumps
    dfsReachable(u);

    // Single jumps
    for (int i = 0; i < 6; i++) {
        if (checkFree(graph[u][i])) {
            reachable[graph[u][i]] = true;
        }
    }

    board[u] = origColor;
}

void clearReachable() {
    memset(reachable, 0, sizeof reachable);
}