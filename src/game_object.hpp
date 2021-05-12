#ifndef GAME_OBJECT_HPP
#define GAME_OBJECT_HPP

#include <SDL2/SDL.h>

class GameObject {
    public:
        GameObject(int x, int y, char** lvl);
        ~GameObject();

        void handleEvent( SDL_Event& e );
        void Update ();

        int xmap;
        int ymap;
        int xpos;
        int ypos;
        int xvel = 0;
        int yvel = 0;

        char** map;
    
    private:

};

#endif