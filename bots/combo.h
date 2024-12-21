// Greedy strategy that plays short sequences of moves with the best average return per turn.
// More sophisticated than basicBot, might be vulnerable to being interrupted during the midgame.

#include <stdlib.h>

#include "../game/dist.h"

int combo_dist[SPACES];

float avg_min;
int combo_len, combo_idx;
int combo_buf[3][2], combo_moves[3][2];

void combo_dfs(int depth, int delta) {
    if (depth >= 1) {
        const float avg_cur = (float) delta / depth;
        if (avg_cur < avg_min) {
            avg_min = avg_cur;
            combo_len = depth;
            memcpy(combo_moves, combo_buf, sizeof combo_buf);
        }
    }

    if (depth < 3) {
        for (int i = 0; i < SPACES; i++) {
            for (int j = 0; j < SPACES; j++) {
                if (hypmove(i, j)) {
                    combo_buf[depth][0] = i;
                    combo_buf[depth][1] = j;
                    combo_dfs(depth + 1, delta + combo_dist[j] - combo_dist[i]);
                    hypmove_undo();
                }
            }
        }
    }
}

void combo_move() {
    static bool init = false;
    if (!init) {
        dist_copy(vertices[(my_color() + 3) % 6], combo_dist);
        init = true;
    }

    if (combo_idx >= combo_len || !move_valid(combo_moves[combo_idx][0], combo_moves[combo_idx][1])) {
        avg_min = 100;
        combo_len = combo_idx = 0;
        combo_dfs(0, 0);
    }

    move(combo_moves[combo_idx][0], combo_moves[combo_idx][1]);
    combo_idx++;
}