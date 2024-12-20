#include "game/game.h"
#include "frontend.h"

#include "bots/randomBot.h"
#include "bots/basicBot.h"
#include "bots/comboBot.h"

int main() {
    initGame();
    // addPlayer(comboMove);
    // addPlayer(basicMove);
    addPlayer(comboMove);
    addPlayer(playerMove);
    runGame();
}