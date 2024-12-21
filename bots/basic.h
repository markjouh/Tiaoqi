// Greedily take the move which covers the most distance.
// Being more or less the simplest thing that can consistently end games, this serves as a baseline for more complex strategies.

#include <stdlib.h>

#include "../game/dist.h"

void basic_move() {
    static bool init = false;
    static int dist[SPACES];
    if (!init) {
        dist_copy(vertices[(my_color() + 3) % 6], dist);
        init = true;
    }

    int min_delta = 100, a, b;
    for (int i = 0; i < SPACES; i++) {
        for (int j = 0; j < SPACES; j++) {
            const int delta = dist[j] - dist[i];
            if (move_valid(i, j) && delta < min_delta) {
                min_delta = delta;
                a = i, b = j;
            }
        }
    }
    assert(move_valid(a, b));
    move(a, b);
}