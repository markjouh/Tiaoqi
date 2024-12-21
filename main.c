#include "game/game.h"
#include "client/client.h"

#include "bots/random.h"
#include "bots/basic.h"
#include "bots/combo.h"

int main() {
    game_init();
    game_add(combo_move);
    game_add(player_move);
    game_run();
}