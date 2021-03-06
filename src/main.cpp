#include "game.hpp"
#include "constants.hpp"

Game* game = nullptr;

int main () {


    game = new Game ();

    game->init("Gaming Machine", SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, SCREEN_WIDTH, SCREEN_HEIGHT, false);

    while (game->running()) {
        game->LoadGameStates();
    }

    game->clean ();
}