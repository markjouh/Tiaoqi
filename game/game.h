#pragma once

#include <stdbool.h>
#include <stdlib.h>
#include <assert.h>
#include <stdio.h>
#include <string.h>

#include "layout.h"

typedef void (*FuncPtr)(void);

// Basic game setup

int board[SPACES];

int players, players_finished, turn;
int player_color[6];
FuncPtr player_func[6];

void game_init() {
    for (int i = 0; i < CENTER; i++) {
        board[center[i]] = -1;
    }
    for (int i = 0; i < 6; i++) {
        for (int j = 0; j < 10; j++) {
            board[corners[i][j]] = i;
        }
    }

    players = players_finished = turn = 0;
    for (int i = 0; i < 3; i++) {
        player_color[2 * i] = i;
        player_color[2 * i + 1] = i + 3;
    }
}

void game_add(FuncPtr func) {
    player_func[players++] = func;
}

int my_color() {
    return player_color[turn % players];
}

bool space_valid(int x) {
    return x >= 0 && x < SPACES;
}

bool space_free(int x) {
    return space_valid(x) && board[x] == -1;
}

bool space_mine(int x) {
    return space_valid(x) && board[x] == my_color();
}

// Jump pathing

int from[SPACES];

void reachable_dfs(int u) {
    for (int i = 0; i < 6; i++) {
        int v = u, dist = 0;

        while (space_free(v)) {
            v = graph[v][i];
            dist++;
        }

        if (v == -1) {
            continue;
        }

        bool ok = true;
        for (int j = 0; j < dist; j++) {
            v = graph[v][i];
            ok &= space_free(v);
        }

        if (ok && from[v] == -1) {
            from[v] = u;
            reachable_dfs(v);
        }
    }
}

void reachable_calc(int u) {
    const int old = board[u];
    board[u] = -1;

    memset(from, -1, sizeof from);

    // Adjacent spaces
    for (int i = 0; i < 6; i++) {
        if (space_free(graph[u][i])) {
            from[graph[u][i]] = u;
        }
    }

    // Long jumps
    reachable_dfs(u);

    // Require a move
    from[u] = -1;

    board[u] = old;
}

void reachable_copy(int u, int *arr) {
    reachable_calc(u);
    memcpy(arr, from, sizeof from);
}

bool move_valid(int a, int b) {
    if (space_mine(a) && space_free(b)) {
        reachable_calc(a);
        return from[b] != -1;
    }

    return false;
}

// Try to make a move, consuming your turn if valid

bool move(int a, int b) {
    if (!move_valid(a, b)) {
        return false;
    }

    board[b] = board[a];
    board[a] = -1;

    turn++;

    return true;
}

// Make a hypothetical move, so we can use built-in helpers when testing possible future states

#define MX_HYPMOVES 100

int hypmoves[MX_HYPMOVES][2];
int hypmove_idx = 0;

bool hypmove(int a, int b) {
    if (!move_valid(a, b)) {
        return false;
    }

    board[b] = board[a];
    board[a] = -1;

    hypmoves[hypmove_idx][0] = a;
    hypmoves[hypmove_idx][1] = b;
    hypmove_idx++;

    return true;
}

void hypmove_undo() {
    hypmove_idx--;
    const int a = hypmoves[hypmove_idx][0], b = hypmoves[hypmove_idx][1];
    board[a] = board[b];
    board[b] = -1;
}

#define MX_TURNS 100

int score(int x) {
    int score = 0;
    for (int i = 0; i < 10; i++) {
        score -= 2 * board[corners[x][i]] == x;
        score += board[corners[(x + 3) % 6][i]] == x;
    }
    return score;
}

bool game_finished() {
    return players_finished == players;
}

void game_iter() {
    if (game_finished()) {
        return;
    }
    
    if (score(my_color()) == 10) {
        turn++;
        return;
    }

    player_func[turn % players]();

    players_finished += score(my_color()) == 10;
    if (turn >= MX_TURNS) {
        players_finished = players;
    }

    while (hypmove_idx > 0) {
        hypmove_undo();
    }
}