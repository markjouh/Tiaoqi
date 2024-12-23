#include "game/game.h"
#include "gui/gui.h"

int main() {
    Game game;
    
    game.add_func(gui_move);
    game.add_func(gui_move);
    
    gui_run(game);
}