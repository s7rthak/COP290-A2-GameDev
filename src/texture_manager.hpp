#ifndef TEXTURE_HPP
#define TEXTURE_HPP

class TextureManager {
    public:
        static SDL_Texture* LoadTexture (const char* fileName);
        static void Draw (SDL_Texture* tex, SDL_Rect src, SDL_Rect dest, float angle, bool flip_vertical, int window);

    private:
};

#endif