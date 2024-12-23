#pragma once

#include <stdbool.h>
#include <stdlib.h>
#include <assert.h>
#include <stdio.h>
#include <string.h>

#include <vector>
#include <queue>
#include <any>
#include <utility>

#include "board.h"

constexpr int MX_TURNS = 200;

struct InputState {
    int turn, color;
    Board board;
};

using MoveFunc = std::pair<int, int>(*)(InputState);

struct Game {
    int turn = 0;
    Board board;
    std::vector<MoveFunc> funcs;
    
    void add_func(MoveFunc func) {
        funcs.push_back(func);
    }
    
    InputState get_state() {
        return {turn, (turn % size(funcs)) / 2 + (turn % 2) * 3, board};
    }
};

void game_run(Game &game) {
    for (int i = 0; i < MX_TURNS; i++) {
        const auto res = game.funcs[game.turn % size(game.funcs)](game.get_state());
        game.board.move(res.first, res.second);
        game.turn++;
    }
}