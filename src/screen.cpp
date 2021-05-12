#include "screen.hpp"
#include <bits/stdc++.h>
#include "screen_component.hpp"
#include "texture_manager.hpp"
#include "constants.hpp"
#include "game.hpp"

Screen::Screen ()
{
    TTF_Init();
}

void Screen::AddComponent(std::string text, SDL_Color textColor, SDL_Color stextColor, int sz, SDL_Rect d, bool transform)
{
    font = TTF_OpenFont("../assets/EvilEmpire-4BBVK.ttf", sz);

    SDL_Surface* textSurface = TTF_RenderText_Solid(font, text.c_str(), textColor);
    ScreenComponent newComponent;
    newComponent.tex = SDL_CreateTextureFromSurface(Game::renderer, textSurface);
    SDL_FreeSurface(textSurface);

    SDL_Surface* stextSurface = TTF_RenderText_Solid(font, text.c_str(), stextColor);
    newComponent.stex = SDL_CreateTextureFromSurface(Game::renderer, stextSurface);
    SDL_FreeSurface(stextSurface);

    int w, h;
    SDL_Rect s;
    s.x = s.y = 0;
    SDL_QueryTexture(newComponent.tex, NULL, NULL, &w, &h);
    s.w = d.w = w; s.h = d.h = h;

    d.x = (SCREEN_WIDTH - w) / 2;

    newComponent.src = s;
    newComponent.dest = d;
    newComponent.transform = transform;

    components.push_back(newComponent);

    TTF_CloseFont(font);
}

void Screen::AddImage (ScreenImage im)
{
    imgs.push_back(im);
}

int Screen::HandleEvents (SDL_Event& e)
{
    int screen_trans = 0, cur_trans;
    for (int i = 0; i < components.size(); i++) {
        if (components[i].transform) {
            cur_trans = components[i].HandleEvents(e);
            if (cur_trans > screen_trans)
                screen_trans = cur_trans;
            if (screen_trans == 2) {
                screen_trans = i + 2;
                break;
            }
        }
    }
    if (screen_trans == SCREEN_REFRESH) Render();
    
    return screen_trans;
}

void Screen::Render () 
{
    SDL_RenderClear(Game::renderer);
    for (int i = 0; i < imgs.size(); i++) {
        TextureManager::Draw(imgs[i].tex, imgs[i].src, imgs[i].dest, 0.0f, false, 0);
    }
    for (int i = 0; i < components.size(); i++) {
        components[i].Render();
    }
    SDL_RenderPresent(Game::renderer);
}