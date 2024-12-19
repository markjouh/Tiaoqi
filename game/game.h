#pragma once

#include <stdbool.h>
#include <stdlib.h>
#include <assert.h>
#include <stdio.h>
#include <string.h>

#include "layout.h"

typedef void (*FuncPtr)(void);

int board[numSpaces];

int players, turn;
int playerColor[6];
FuncPtr playerFuncs[6];

void initGame() {
    for (int i = 0; i < numCenter; i++) {
        board[center[i]] = -1;
    }
    for (int i = 0; i < 6; i++) {
        for (int j = 0; j < 10; j++) {
            board[corners[i][j]] = i;
        }
    }

    players = turn = 0;
    for (int i = 0; i < 3; i++) {
        playerColor[2 * i] = i;
        playerColor[2 * i + 1] = i + 3;
    }
}

void addPlayer(FuncPtr func) {
    playerFuncs[players++] = func;
}

int myColor() {
    return playerColor[turn % players];
}

bool chkValid(int x) {
    return x >= 0 && x < numSpaces;
}

bool chkFree(int x) {
    return chkValid(x) && board[x] == -1;
}

bool chkMine(int x) {
    return chkValid(x) && board[x] == myColor();
}

int reachFrom[numSpaces];

void dfsReachable(int u) {
    for (int i = 0; i < 6; i++) {
        int v = u, dist = 0;

        while (chkFree(v)) {
            v = graph[v][i];
            dist++;
        }

        if (v == -1) {
            continue;
        }

        bool ok = true;
        for (int j = 0; j < dist; j++) {
            v = graph[v][i];
            ok &= chkFree(v);
        }

        if (ok && reachFrom[v] == -1) {
            reachFrom[v] = u;
            dfsReachable(v);
        }
    }
}

void calcReachable(int u, int *arr) {
    const int origColor = board[u];
    board[u] = -1;

    memset(reachFrom, -1, sizeof reachFrom);

    // Adjacent spaces
    for (int i = 0; i < 6; i++) {
        if (chkFree(graph[u][i])) {
            reachFrom[graph[u][i]] = u;
        }
    }

    // Long jumps
    dfsReachable(u);

    // Require a move
    reachFrom[u] = -1;

    memcpy(arr, reachFrom, sizeof reachFrom);

    board[u] = origColor;
}

bool validMove(int a, int b) {
    if (chkMine(a) && chkFree(b)) {
        int from[numSpaces];
        calcReachable(a, from);

        return from[b] != -1;
    }

    return false;
}

bool makeMove(int a, int b) {
    if (!validMove(a, b)) {
        return false;
    }

    board[b] = board[a];
    board[a] = -1;

    turn++;

    return true;
}

void gameLoop() {
    playerFuncs[turn % players]();
}