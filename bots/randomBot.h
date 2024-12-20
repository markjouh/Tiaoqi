// Uniformly choose a random valid move.
// Mostly used as a placeholder player.

#include <time.h>

void randomMove() {
    srand(time(NULL));

    while (true) {
        if (makeMove(rand() % numSpaces, rand() % numSpaces)) {
            return;
        }
    }
}