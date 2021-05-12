#ifndef SCREEN_COMPONENT_HPP
#define SCREEN_COMPONENT_HPP

#include <SDL2/SDL.h>

class ScreenComponent{
    public:

        ScreenComponent ();

        SDL_Texture* tex;
        SDL_Texture* stex;
        bool transform;
        SDL_Rect src;
        SDL_Rect dest;
        bool show_alt = false;

        int HandleEvents (SDL_Event& e);
        void Render ();

    private:

};

struct ScreenImage{
    SDL_Texture* tex;
    SDL_Rect src;
    SDL_Rect dest;
};

#endif