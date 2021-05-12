#include "game_object.hpp"
#include "game.hpp"
#include "constants.hpp"
#include "texture_manager.hpp"

GameObject::GameObject(int x, int y, char** lvl) {
    // objTexture = TextureManager::LoadTexture(texturesheet);

    xmap = x;
    ymap = y;

    xpos = x * TILE_WIDTH;
    ypos = y * TILE_HEIGHT;

    map = lvl;
}

void GameObject::handleEvent (SDL_Event &e) 
{
    if (e.type == SDL_KEYDOWN && e.key.repeat == 0) {
        switch (e.key.keysym.sym)
        {
            case SDLK_RIGHT:
                xvel = MOVE_VEL_X;
                yvel = 0;
                break;
            
            case SDLK_LEFT:
                xvel = -MOVE_VEL_X;
                yvel = 0;
                break;

            case SDLK_UP:
                xvel = 0;
                yvel = -MOVE_VEL_Y;
                break;
            
            case SDLK_DOWN:
                xvel = 0;
                yvel = MOVE_VEL_Y;
                break;
            
            default:
                break;
        }
    }    
}

void GameObject::Update()
{
    int xpos_new, ypos_new;

    xpos_new = xpos + xvel;
    ypos_new = ypos + yvel;

    int xpos_test_right = xpos_new + (TILE_WIDTH - 1);
    int ypos_test_down = ypos_new + (TILE_HEIGHT - 1);
    
    if (map[ypos_new / TILE_HEIGHT][xpos_test_right / TILE_WIDTH] == WALL || map[ypos_new / TILE_HEIGHT][xpos_new / TILE_WIDTH] == WALL || map[ypos_test_down / TILE_HEIGHT][xpos_new / TILE_WIDTH] == WALL || map[ypos_test_down / TILE_HEIGHT][xpos_test_right / TILE_WIDTH] == WALL) {
        xpos = xmap * TILE_WIDTH;
        ypos = ymap * TILE_HEIGHT;
    } 
    else {
        xpos = xpos_new;
        ypos = ypos_new;
    }

    int xmap_new, ymap_new;

    xmap_new = (xpos + TILE_WIDTH / 2) / TILE_WIDTH;
    ymap_new = (ypos + TILE_HEIGHT / 2) / TILE_HEIGHT;

    if (xmap_new != xmap || ymap_new != ymap) {
        map[ymap][xmap] = EMPTY;
        
        map[ymap_new][xmap_new] = PACMAN;
    }

    xmap = xmap_new;
    ymap = ymap_new;
}