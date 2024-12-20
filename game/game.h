#pragma once

#include <stdbool.h>
#include <stdlib.h>
#include <assert.h>
#include <stdio.h>
#include <string.h>

#include "layout.h"

typedef void (*FuncPtr)(void);

// Basic game setup

int board[numSpaces];

int players, turn;
int playerColor[6];
FuncPtr playerFuncs[6];

int finishedPlayers;

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

    finishedPlayers = 0;
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

// Jump pathing

int from[numSpaces];

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

        if (ok && from[v] == -1) {
            from[v] = u;
            dfsReachable(v);
        }
    }
}

void calcReachable(int u) {
    const int origColor = board[u];
    board[u] = -1;

    memset(from, -1, sizeof from);

    // Adjacent spaces
    for (int i = 0; i < 6; i++) {
        if (chkFree(graph[u][i])) {
            from[graph[u][i]] = u;
        }
    }

    // Long jumps
    dfsReachable(u);

    // Require a move
    from[u] = -1;

    board[u] = origColor;
}

void getReachable(int u, int *arr) {
    calcReachable(u);
    memcpy(arr, from, sizeof from);
}

bool validMove(int a, int b) {
    if (chkMine(a) && chkFree(b)) {
        calcReachable(a);
        return from[b] != -1;
    }

    return false;
}

// Try to make a move, consuming your turn if valid

bool makeMove(int a, int b) {
    if (!validMove(a, b)) {
        return false;
    }

    board[b] = board[a];
    board[a] = -1;

    turn++;

    return true;
}

// Make a hypothetical move, so we can use built-in helpers when testing possible future states

const int maxHypDepth = 100;

int hypMoves[maxHypDepth][2];
int hypIdx = 0;

bool makeHypMove(int a, int b) {
    if (!validMove(a, b)) {
        return false;
    }

    board[b] = board[a];
    board[a] = -1;

    hypMoves[hypIdx][0] = a;
    hypMoves[hypIdx][1] = b;
    hypIdx++;

    return true;
}

void undoHypMove() {
    hypIdx--;
    const int a = hypMoves[hypIdx][0], b = hypMoves[hypIdx][1];
    board[a] = board[b];
    board[b] = -1;
}

const int maxTurns = 100;

int getScore(int x) {
    int score = 0;
    for (int i = 0; i < 10; i++) {
        score -= 2 * board[corners[x][i]] == x;
        score += board[corners[(x + 3) % 6][i]] == x;
    }
    return score;
}

bool gameFinished() {
    return finishedPlayers == players;
}

void gameLoop() {
    if (gameFinished()) {
        return;
    }
    
    if (getScore(myColor()) == 10) {
        turn++;
        return;
    }

    playerFuncs[turn % players]();

    finishedPlayers += getScore(myColor()) == 10;
    if (turn >= maxTurns) {
        finishedPlayers = players;
    }

    while (hypIdx > 0) {
        undoHypMove();
    }
}