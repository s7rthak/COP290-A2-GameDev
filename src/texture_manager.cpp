#include "texture_manager.hpp"

SDL_Texture* TextureManager::LoadTexture(const char* fileName) {
    SDL_Surface* tempSurface = IMG_Load(fileName);
    SDL_Texture* tex = SDL_CreateTextureFromSurface(Game::renderer, tempSurface);
    SDL_FreeSurface(tempSurface);

    return tex;
}

void TextureManager::Draw(SDL_Texture* tex, SDL_Rect src, SDL_Rect dest, float angle, bool flip_horizontal) {
    if (flip_horizontal) {
        SDL_RenderCopyEx(Game::renderer, tex, &src, &dest, angle, NULL, SDL_FLIP_VERTICAL);
    }
    else {
        SDL_RenderCopyEx(Game::renderer, tex, &src, &dest, angle, NULL, SDL_FLIP_NONE);
    }
}