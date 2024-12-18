#include "game/game.h"
#include "frontend.h"

#include "bots/randomBot.h"
#include "bots/basicBot.h"

int main() {
    initGame();
    addPlayer(playerMove);
    addPlayer(basicMove);
    addPlayer(randomMove);
    addPlayer(randomMove);

    runGame();
}