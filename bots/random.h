// Uniformly choose a random valid move.
// Mostly used as a placeholder player.

#include <time.h>

void random_move() {
    srand(time(NULL));

    while (true) {
        if (move(rand() % SPACES, rand() % SPACES)) {
            return;
        }
    }
}