#include "screen_component.hpp"
#include "texture_manager.hpp"
#include "constants.hpp"

ScreenComponent::ScreenComponent ()
{

}



int ScreenComponent::HandleEvents (SDL_Event& e) 
{
    if (e.type == SDL_MOUSEMOTION) {
        int x, y;
        SDL_PumpEvents();
        SDL_GetMouseState(&x, &y);
        bool inside = true;
        if (x < dest.x) inside = false;
        else if (x > dest.x + dest.w) inside = false;
        else if (y < dest.y) inside = false;
        else if (y > dest.y + dest.h) inside = false;

        if(show_alt != inside) {
            // std::cout << x << " " << y << " " << (int)show_alt << "\n";
            show_alt = inside;
            return SCREEN_REFRESH;
        }
        return SCREEN_SAME;
    } 
    else if (e.type == SDL_MOUSEBUTTONDOWN) {
        int x, y;
        SDL_PumpEvents();
        SDL_GetMouseState(&x, &y);
        bool inside = true;
        if (x < dest.x) inside = false;
        else if (x > dest.x + dest.w) inside = false;
        else if (y < dest.y) inside = false;
        else if (y > dest.y + dest.h) inside = false;

        if (inside) return SCREEN_CHANGE;
        return SCREEN_SAME;
    }
    return SCREEN_SAME;
}

void ScreenComponent::Render ()
{
    if (show_alt) {
        TextureManager::Draw(stex, src, dest, 0.0f, false, 0);
    } else {
        TextureManager::Draw(tex, src, dest, 0.0f, false, 0);
    }
    // SDL_RenderPresent(Game::renderer);
}