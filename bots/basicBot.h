// Greedily take the move which covers the most distance.
// Being more or less the simplest thing that can consistently end games, this serves as a baseline for more complex strategies.

#include <stdlib.h>

#include "../game/dist.h"

void basicMove() {
    static bool init = false;
    static int dist[numSpaces];
    if (!init) {
        getDists(vertices[(myColor() + 3) % 6], dist);
        init = true;
    }

    int minDelta = 100, a, b;
    for (int i = 0; i < numSpaces; i++) {
        for (int j = 0; j < numSpaces; j++) {
            const int delta = dist[j] - dist[i];
            if (validMove(i, j) && delta < minDelta) {
                minDelta = delta;
                a = i, b = j;
            }
        }
    }
    assert(validMove(a, b));
    makeMove(a, b);
}