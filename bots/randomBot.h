#include <time.h>

void randomMove() {
    srand(time(NULL));

    while (true) {
        if (makeMove(rand() % numSpaces, rand() % numSpaces)) {
            return;
        }
    }
}