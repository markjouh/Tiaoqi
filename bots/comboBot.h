// Greedy strategy that plays short sequences of moves with the best average return per turn.
// More sophisticated than basicBot, might be vulnerable to being interrupted during the midgame.

#include <stdlib.h>

#include "../game/dist.h"

int comboDist[numSpaces];

float minAvg;
int comboLen, comboIdx;
int curMoves[3][2], resMoves[3][2];

void comboDfs(int depth, int delta) {
    if (depth >= 1) {
        const float curAvg = (float) delta / depth;
        if (curAvg < minAvg) {
            minAvg = curAvg;
            comboLen = depth;
            memcpy(resMoves, curMoves, sizeof curMoves);
        }
    }

    if (depth < 3) {
        for (int i = 0; i < numSpaces; i++) {
            for (int j = 0; j < numSpaces; j++) {
                if (makeHypMove(i, j)) {
                    curMoves[depth][0] = i;
                    curMoves[depth][1] = j;
                    comboDfs(depth + 1, delta + comboDist[j] - comboDist[i]);
                    undoHypMove();
                }
            }
        }
    }
}

void comboMove() {
    static bool init = false;
    if (!init) {
        getDists(vertices[(myColor() + 3) % 6], comboDist);
        init = true;
    }

    if (comboIdx >= comboLen || !validMove(resMoves[comboIdx][0], resMoves[comboIdx][1])) {
        minAvg = 100;
        comboLen = comboIdx = 0;
        comboDfs(0, 0);
    }

    makeMove(resMoves[comboIdx][0], resMoves[comboIdx][1]);
    comboIdx++;
}