#include "game.hpp"
#include "texture_manager.hpp"
#include <SDL2/SDL_image.h>

SDL_Texture* TextureManager::LoadTexture(const char* fileName) {
    SDL_Surface* tempSurface = IMG_Load(fileName);
    SDL_Texture* tex = SDL_CreateTextureFromSurface(Game::renderer, tempSurface);
    SDL_FreeSurface(tempSurface);

    return tex;
}

void TextureManager::Draw(SDL_Texture* tex, SDL_Rect src, SDL_Rect dest, float angle, bool flip_horizontal, int window) {
    if (window == 0) {
        if (flip_horizontal) {
            SDL_RenderCopyEx(Game::renderer, tex, &src, &dest, angle, NULL, SDL_FLIP_VERTICAL);
        }
        else {
            SDL_RenderCopyEx(Game::renderer, tex, &src, &dest, angle, NULL, SDL_FLIP_NONE);
        }
    }
    else if (window == 1) {
        if (flip_horizontal) {
            SDL_RenderCopyEx(Game::nRenderer, tex, &src, &dest, angle, NULL, SDL_FLIP_VERTICAL);
        }
        else {
            SDL_RenderCopyEx(Game::nRenderer, tex, &src, &dest, angle, NULL, SDL_FLIP_NONE);
        }
    }
}