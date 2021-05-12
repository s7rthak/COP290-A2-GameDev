#ifndef GAME_HPP
#define GAME_HPP

#include <SDL2/SDL.h>


class Game {
    public:
        Game();
        ~Game();

        void init (const char* title, int xpos, int ypos, int width, int height, bool fullscreen);

        void LoadGameStates ();
        void handleEvents ();
        void update ();
        void render ();
        void clean ();

        void CheckGoodEndGame ();
        void CheckBadEndGame ();
        void UpdateScore ();

        bool running () { return isRunning; }
        
        static SDL_Renderer* renderer;
        static SDL_Renderer* nRenderer;

        int lives_left;

    private:
        bool isRunning;
        int gameState;
        SDL_Window* window;
};

#endif /* Game_hpp */