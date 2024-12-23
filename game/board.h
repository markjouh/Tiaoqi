#pragma once

#include <array>

#include "constants.h"

struct Board {
    std::array<int, SPACES> board;
    
    int &operator[](size_t i) {
        return board[i];
    }
    
    Board() {
        for (int i = 0; i < CENTER; i++) {
            board[center[i]] = -1;
        }
        for (int i = 0; i < 6; i++) {
            for (int j = 0; j < 10; j++) {
                board[corners[i][j]] = i;
            }
        }
    }
    
    bool valid(int u) const {
        return u >= 0 && u < SPACES;
    }
    
    bool free(int u) const {
        return valid(u) && board[u] == -1;
    }
    
    int score(int color) const {
        int res = 0;
        for (int i = 0; i < 10; i++) {
            res -= 2 * board[corners[color][i]] == color;
            res += board[corners[(color + 3) % 6][i]] == color;
        }
        return res;
    }
    
    void move(int u, int v) {
        board[v] = board[u];
        board[u] = -1;
    }
    
    auto get_dists(int u);
    auto get_moves(int u);
};

#include "dists.h"

auto Board::get_dists(int u) {
    return __get_dists(u);
}

#include "moves.h"

auto Board::get_moves(int u) {
    return __get_moves(*this, u);
}