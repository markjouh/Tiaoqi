bool reachable[width][width];

bool checkFree(int x, int y) {
    return x >= 0 && x < width && y >= 0 && y < width && board[x][y] == -1;
}

void dfsReachable(int x, int y) {
    reachable[x][y] = true;

    for (int i = 0; i < 6; i++) {
        int nX = x, nY = y, dist = 0;

        while (checkFree(nX, nY)) {
            nX += deltaX[i];
            nY += deltaY[i];
            dist++;
        }

        bool ok = true;
        for (int j = 0; j < dist; j++) {
            nX += deltaX[i];
            nY += deltaY[i];
            ok &= checkFree(nX, nY);
        }

        if (ok && !reachable[nX][nY]) {
            dfsReachable(nX, nY);
        }
    }
}

void findReachable(int pos) {
    const int x = spaces[pos].x, y = spaces[pos].y;

    const int origColor = board[x][y];
    board[x][y] = -1;

    // Multi-jumps
    dfsReachable(x, y);

    // Single jumps
    for (int i = 0; i < 6; i++) {
        const int nX = x + deltaX[i], nY = y + deltaY[i];
        if (checkFree(nX, nY)) {
            reachable[nX][nY] = true;
        }
    }

    board[x][y] = origColor;
}

void clearReachable() {
    memset(reachable, 0, sizeof reachable);
}