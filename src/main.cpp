#include "game.hpp"
#include "constants.hpp"

Game* game = nullptr;

int main () {


    game = new Game ();

    game->init("TestingMachine", SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, SCREEN_WIDTH, SCREEN_HEIGHT, false);

    while (game->running()) {
        game->LoadGameStates();
        // game->handleEvents();
        // game->CheckGoodEndGame();
        // game->update ();
        // game->CheckBadEndGame();
        // game->render ();
    }

    game->clean ();
}