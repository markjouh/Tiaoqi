void basicMove() {
    static bool init = false;
    if (!init) {
        init = true;
    }

    srand(time(0));
    while (true) {
        if (makeMove(rand() % numSpaces, rand() % numSpaces)) {
            return;
        }
    }
}