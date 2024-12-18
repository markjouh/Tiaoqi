#include "../game/absDist.h"

int basicDist[numSpaces];

void basicMove() {
    static bool init = false;
    if (!init) {
        const int oppColor = (myColor() + 3) % 6;

        int root = -1;
        for (int i = 0; i < 10; i++) {
            int deg = 0;
            for (int j = 0; j < 6; j++) {
                deg += graph[corners[oppColor][i]][j] != -1;
            }
            if (deg == 2) {
                root = corners[oppColor][i];
                break;
            }
        }
        assert(root != -1);

        findAbsDists(root);
        memcpy(basicDist, absDist, sizeof absDist);

        init = true;
    }

    int best = 999;
    int a, b;
    for (int i = 0; i < numSpaces; i++) {
        for (int j = 0; j < numSpaces; j++) {
            const int delta = basicDist[j] - basicDist[i];
            if (validMove(i, j) && delta < best) {
                best = delta;
                a = i, b = j;
            }
        }
    }
    assert(validMove(a, b));
    makeMove(a, b);
}