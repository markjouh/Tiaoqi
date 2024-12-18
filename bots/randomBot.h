void randomMove() {
    srand(time(0));
    while (true) {
        if (makeMove(rand() % numSpaces, rand() % numSpaces)) {
            return;
        }
    }
}