#ifndef SCREEN_HPP
#define SCREEN_HPP

#include <SDL2/SDL_ttf.h>
#include <bits/stdc++.h>
#include "screen_component.hpp"

class Screen {
    public:
        Screen ();
        ~Screen ();

        void AddComponent (std::string text, SDL_Color textColor, SDL_Color stextColor, int sz, SDL_Rect d, bool transform);
        void AddImage (ScreenImage im);
        int HandleEvents (SDL_Event& e);
        void Render ();

        TTF_Font* font;
        std::vector <ScreenComponent> components;
        std::vector <ScreenImage> imgs;

    private:
        
};

#endif