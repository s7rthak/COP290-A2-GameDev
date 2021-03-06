#include "map.hpp"
#include "game.hpp"
#include "texture_manager.hpp"
#include "constants.hpp"
#include "util.hpp"
#include <SDL2/SDL_ttf.h>

Map::Map (char** lvl, std::vector<int> &posi) 
{
    std::random_device dev;
    std::mt19937 rng(dev());
    std::uniform_int_distribution<std::mt19937::result_type> dist6(1,6); // distribution in range [1, 6]

    int choose = dist6(rng);

    std::string WallChosen = "../assets/wall-" + std::to_string(choose) + ".png";

    wall = TextureManager::LoadTexture(WallChosen.c_str());
    food = TextureManager::LoadTexture("../assets/food.png");
    pacman_fo = TextureManager::LoadTexture("../assets/pacman_full_open.png");
    pacman_ho = TextureManager::LoadTexture("../assets/pacman_half_open.png");
    pacman_c = TextureManager::LoadTexture("../assets/pacman_close.png");
    monster1 = TextureManager::LoadTexture("../assets/monster1.png");
    monster2 = TextureManager::LoadTexture("../assets/monster2.png");

    LoadMap(lvl);

    src.x = src.y = 0;
    src.w = dest.w = TILE_WIDTH;
    src.h = dest.h = TILE_HEIGHT;

    Pacman = new GameObject(posi[1], posi[0], lvl);
    Monster1 = new GameObject(posi[3], posi[2], lvl);
    Monster2 = new GameObject(posi[5], posi[4], lvl);
}

void Map::LoadMap (char** lvl) 
{
    map = lvl;
}

void Map::UpdateMap (SDL_Event &e) 
{
    Pacman->handleEvent (e);
    Pacman->Update ();
}

void Map::MoveMonsters ()
{
    int xmap_new, ymap_new;
    std::pair <int, std::string> res;
    std::string action;

    // Move Monster1
    // std::cout << Monster1->xpos << "\n";
    if (tmp1 == 0) {
        res = ChooseGreedyActionMD(Monster1, Monster2, Pacman, DIFFICULTY, FRAME_JUMP, tmp1, lma1);
        tmp1 = res.first;
        lma1 = res.second;
    } 
    else {
        tmp1--;
    }
    action = lma1;

    if (action == "UP") {
        Monster1->ypos = Monster1->ypos - FRAME_JUMP * MOVE_VEL_Y;
    }
    else if (action == "DOWN") {
        Monster1->ypos = Monster1->ypos + FRAME_JUMP * MOVE_VEL_Y;
    }
    else if (action == "RIGHT") {
        Monster1->xpos = Monster1->xpos + FRAME_JUMP * MOVE_VEL_X;
    }
    else if (action == "LEFT") {
        Monster1->xpos = Monster1->xpos - FRAME_JUMP * MOVE_VEL_X;
    }

    xmap_new = (Monster1->xpos + TILE_WIDTH / 2) / TILE_WIDTH;
    ymap_new = (Monster1->ypos + TILE_HEIGHT / 2) / TILE_HEIGHT;

    if (xmap_new != Monster1->xmap || ymap_new != Monster1->ymap) {
        if (map[Monster1->ymap][Monster1->xmap] == MONSTER1_ON_EMPTY) map[Monster1->ymap][Monster1->xmap] = EMPTY;
        else map[Monster1->ymap][Monster1->xmap] = FOOD;
        if (map[ymap_new][xmap_new] == FOOD) map[ymap_new][xmap_new] = MONSTER1_ON_FOOD;
        else map[ymap_new][xmap_new] = MONSTER1_ON_EMPTY;
    }

    Monster1->xmap = xmap_new;
    Monster1->ymap = ymap_new;

    // Move Monster2
    if (tmp2 == 0) {
        res = ChooseGreedyActionMD(Monster2, Monster1, Pacman, DIFFICULTY, FRAME_JUMP, tmp2, lma2);
        tmp2 = res.first;
        lma2 = res.second;
    } 
    else {
        tmp2--;
    }
    action = lma2;

    if (action == "UP") {
        Monster2->ypos = Monster2->ypos - FRAME_JUMP * MOVE_VEL_Y;
    }
    else if (action == "DOWN") {
        Monster2->ypos = Monster2->ypos + FRAME_JUMP * MOVE_VEL_Y;
    }
    else if (action == "RIGHT") {
        Monster2->xpos = Monster2->xpos + FRAME_JUMP * MOVE_VEL_X;
    }
    else if (action == "LEFT") {
        Monster2->xpos = Monster2->xpos - FRAME_JUMP * MOVE_VEL_X;
    }

    xmap_new = (Monster2->xpos + TILE_WIDTH / 2) / TILE_WIDTH;
    ymap_new = (Monster2->ypos + TILE_HEIGHT / 2) / TILE_HEIGHT;

    if (xmap_new != Monster2->xmap || ymap_new != Monster2->ymap) {
        if (map[Monster2->ymap][Monster2->xmap] == MONSTER2_ON_EMPTY) map[Monster2->ymap][Monster2->xmap] = EMPTY;
        else map[Monster2->ymap][Monster2->xmap] = FOOD;
        if (map[ymap_new][xmap_new] == FOOD) map[ymap_new][xmap_new] = MONSTER2_ON_FOOD;
        else map[ymap_new][xmap_new] = MONSTER2_ON_EMPTY;
    }

    Monster2->xmap = xmap_new;
    Monster2->ymap = ymap_new;
}

void Map::DrawMap () 
{
    for (int i = 0; i < 31; i++) {
        for (int j = 0; j < 31; j++) {
            if (map[i][j] == WALL && map[i+1][j] == WALL && map[i][j+1] == WALL && map[i+1][j+1] == WALL) {
                dest.x = j * TILE_WIDTH + TILE_WIDTH / 2;
                dest.y = i * TILE_HEIGHT + TILE_HEIGHT / 2;

                TextureManager::Draw(wall, src, dest, 0.0f, false, 0);
            }
            if (map[i][j] == FOOD) {
                dest.x = j * TILE_WIDTH;
                dest.y = i * TILE_HEIGHT;
                TextureManager::Draw(food, src, dest, 0.0f, false, 0);
            }
        }
    }

    for (int i = 0; i < 32; i++) {
        for (int j = 0; j < 32; j++) {
            if (map[i][j] == PACMAN) {
                dest.x = Pacman->xpos;
                dest.y = Pacman->ypos;

                double angle = 0.0f;
                bool flip = false;

                if (Pacman->xvel == -MOVE_VEL_X) {
                    angle = 180.0f;
                    flip = true;
                } 
                else if (Pacman->yvel == MOVE_VEL_Y) {
                    angle = 90.0f;
                }
                else if (Pacman->yvel == -MOVE_VEL_Y) {
                    angle = 270.0f;
                }

                switch ((FrameCnt / 10) % 3)
                {
                    case 0:
                        TextureManager::Draw(pacman_fo, src, dest, angle, flip, 0);
                        break;
                    
                    case 1:
                        TextureManager::Draw(pacman_ho, src, dest, angle, flip, 0);
                        break;

                    case 2:
                        TextureManager::Draw(pacman_c, src, dest, angle, flip, 0);
                        break;

                    default:
                        break;
                }
            }
            else if (map[i][j] == MONSTER1_ON_EMPTY || map[i][j] == MONSTER1_ON_FOOD) {
                dest.x = Monster1->xpos;
                dest.y = Monster1->ypos;
                TextureManager::Draw(monster1, src, dest, 0.0f, false, 0);
            }
            else if (map[i][j] == MONSTER2_ON_EMPTY || map[i][j] == MONSTER2_ON_FOOD) {
                dest.x = Monster2->xpos;
                dest.y = Monster2->ypos;
                TextureManager::Draw(monster2, src, dest, 0.0f, false, 0);
            }
        }
    }

    FrameCnt++;
}

void Map::RenderMap () 
{
    SDL_Rect s, d;
    SDL_RenderClear(Game::renderer);

    DrawMap();

    TTF_Font* font = TTF_OpenFont("../assets/EvilEmpire-4BBVK.ttf", 50);

    // Set-up score-board texture.
    std::string text = "Score: " + std::to_string(score);
    SDL_Surface* textSurface = TTF_RenderText_Solid(font, text.c_str(), { 0, 255, 255 });
    SDL_Texture* score_board = SDL_CreateTextureFromSurface(Game::renderer, textSurface);
    SDL_FreeSurface(textSurface);

    int w, h;
    SDL_QueryTexture(score_board, NULL, NULL, &w, &h);

    s.x = s.y = 0; d.x = 20; d.y = 780; s.w = d.w = w; s.h = d.h = h;
    TextureManager::Draw(score_board, s, d, 0.0f, false, 0);

    TTF_CloseFont(font);

    // Set-up Lives texture.
    SDL_Texture* life = TextureManager::LoadTexture("../assets/life.png");
    SDL_QueryTexture(life, NULL, NULL, &w, &h);

    s.x = s.y = 0; s.w = d.w = w; s.h = d.h = h; d.y = 780;
    for (int i = 0; i < lives_left; i++) {
        d.x = SCREEN_WIDTH - (i + 1) * (w - 3);
        TextureManager::Draw(life, s, d, 0.0f, false, 0); 
    }

    SDL_RenderPresent(Game::renderer);
}