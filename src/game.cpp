#include "game.hpp"
#include <SDL2/SDL_image.h>
#include <bits/stdc++.h>
#include "texture_manager.hpp"
#include "game_object.hpp"
#include "map.hpp"
#include "map_layout_read.hpp"
#include "screen.hpp"
#include "two_player_map.hpp"
#include "constants.hpp"
#include <SDL2/SDL_ttf.h>


int MD (int x1, int y1, int x2, int y2) {
    return abs(x1 - x2) + abs(y1 - y2);
}

// GameObject* pacman;
// GameObject* monster1;
// GameObject* monster2;

SDL_Renderer* Game::renderer = nullptr;
SDL_Renderer* Game::nRenderer = nullptr;
Map* map;
TwoPlayerMap* two_player;
Screen* firstScreen;
Screen* goodEnd;
Screen* badEnd; 

Uint32 frameStart;
int frameTime;

SDL_Rect s, d;

Game::Game()
{}

Game::~Game()
{}

void Game::init (const char* title, int xpos, int ypos, int width, int height, bool fullscreen)
{
    int flags = 0;
    if (fullscreen) {
        flags = SDL_WINDOW_FULLSCREEN;
    }

    if (SDL_Init(SDL_INIT_EVERYTHING) == 0) {
        std::cout << "Subsystem Initialised!...\n";

        window = SDL_CreateWindow(title, xpos, ypos, width, height, flags);
        if (window) {
            std::cout << "Window created!\n";
        }

        renderer = SDL_CreateRenderer(window, -1, 0);
        if (renderer) {
            // SDL_SetRenderDrawColor(renderer, 255, 255, 255, 255);
            std::cout << "Renderer created!\n";
        }

        isRunning = true;
    } else {
        isRunning = false;
    }
}

void Game::LoadGameStates () 
{
    gameState = 0;
    int he_res;
    SDL_Event event;

    while (gameState != -1) {

        if (gameState == START_MENU) {
            firstScreen = new Screen();

            d.y = 50;
            firstScreen->AddComponent("PACMAN RE-IMAGINED", { 255, 255, 0 }, { 255, 255, 0 }, 100, d, false);
            
            d.y = 300;
            firstScreen->AddComponent("Single Player", { 255, 0, 0 }, { 0, 255, 0 }, 50, d, true);

            d.y = 360;
            firstScreen->AddComponent("Two Player", { 255, 0, 0 }, {0, 255, 0}, 50, d, true);

            d.y = 420;
            firstScreen->AddComponent("Black Box", { 255, 0, 0 }, { 0, 255, 0 }, 50, d, true);

            d.y = 660;
            firstScreen->AddComponent("Game by - Sir T Hawk", { 0, 0, 255 }, { 0, 0, 255 }, 30, d, false);

            firstScreen->Render ();
            
            while (gameState == START_MENU){
                while (SDL_PollEvent(&event) != 0) {
                    switch (event.type)
                    {
                        case SDL_QUIT:
                            gameState = EXITED;
                            isRunning = false;
                            break;
                        
                        default:
                            he_res = firstScreen->HandleEvents (event);
                            if (he_res == SCREEN_CHANGE_SP) {
                                gameState = SINGLE_PLAYER;
                            } 
                            else if (he_res == SCREEN_CHANGE_2P) {
                                gameState = TWO_PLAYER;
                            }
                            else if (he_res == SCREEN_CHANGE_BB) {
                                gameState = BLACK_BOX;
                            }
                            break;
                    }
                }
            }
        }

        else if (gameState == SINGLE_PLAYER) {
            std::system("./maze_gen");

            // dynamically create array of pointers of size num_rows
            char** rand_lvl = (char**)malloc(32 * sizeof(char*));
        
            // dynamically allocate memory of size num_rows for each row
            for (int i = 0; i < 32; i++)
                rand_lvl[i] = (char*)malloc(32 * sizeof(char));

            std::vector <int> pos = ReadLayout(rand_lvl, "../assets/maze_layout.txt");
            map = new Map(rand_lvl, pos);
            map->lives_left = MAX_LIVES;

            map->RenderMap();
            SDL_Delay(3000);

            while (gameState == SINGLE_PLAYER) {
                frameStart = SDL_GetTicks();
                SDL_PollEvent(&event);

                switch (event.type)
                {
                case SDL_QUIT:
                    gameState = EXITED;
                    isRunning = false;
                    break;

                default:
                    map->UpdateMap(event);
                    CheckGoodEndGame();
                    update();
                    UpdateScore ();
                    CheckBadEndGame();
                    map->RenderMap();

                    break;
                }

                frameTime = SDL_GetTicks() - frameStart;
                if (frameDelay > frameTime) {
                    SDL_Delay(frameDelay - frameTime);
                }
            }
        }

        else if (gameState == TWO_PLAYER) {
            // dynamically create array of pointers of size num_rows
            char** lvl = (char**)malloc(32 * sizeof(char*));
        
            // dynamically allocate memory of size num_rows for each row
            for (int i = 0; i < 32; i++)
                lvl[i] = (char*)malloc(32 * sizeof(char));

            two_player = new TwoPlayerMap(lvl);
            SDL_Delay(3000);
            two_player->RenderMap();

            while (gameState == TWO_PLAYER) {
                frameStart = SDL_GetTicks();
                SDL_PollEvent(&event);

                switch (event.type)
                {
                case SDL_QUIT:
                    gameState = EXITED;
                    isRunning = false;
                    break;

                default:
                    two_player->RenderMap ();
                    two_player->ExchangeMapInfo ();
                    break;
                }

                frameTime = SDL_GetTicks() - frameStart;
                if (frameDelay > frameTime) {
                    SDL_Delay(frameDelay - frameTime);
                }
            }
        }

        else if (gameState == GOOD_END_SP) {
            goodEnd = new Screen();
            SDL_Texture* youWin = TextureManager::LoadTexture("../assets/win.png");

            ScreenImage winner;
            winner.tex = youWin;
            s.x = 0; s.y = 100; d.x = d.y = 0;
            s.w = d.w = SCREEN_WIDTH; s.h = d.h = SCREEN_HEIGHT - 100;
            winner.src = s;
            winner.dest = d;

            d.y = 100;
            goodEnd->AddComponent("Your Score: " + std::to_string(map->score + 1000), { 0, 0, 255 }, { 0, 0, 255 }, 60, d, false);

            goodEnd->AddImage(winner); 
            goodEnd->Render();

            SDL_Delay(4000);

            d.y = 600;
            goodEnd->AddComponent("Play Again", { 0, 255, 255 }, { 255, 0, 255 }, 50, d, true);

            d.y = 660;
            goodEnd->AddComponent("Exit", { 0, 255, 255 }, { 255, 0, 255 }, 50, d, true);

            goodEnd->Render();

            while (gameState == GOOD_END_SP) {
                while (SDL_PollEvent(&event) != 0) {
                    switch (event.type)
                    {
                        case SDL_QUIT:
                            gameState = EXITED;
                            isRunning = false;
                            break;
                        
                        default:
                            he_res = goodEnd->HandleEvents (event);
                            if (he_res == 3) {
                                gameState = START_MENU;
                                map->lives_left = MAX_LIVES;
                            }
                            else if (he_res == 4) {
                                gameState = EXITED;
                                isRunning = false;
                            }
                            break;
                    }
                }
            }
        }

        else if (gameState == BAD_END_SP) {
            badEnd = new Screen();
            SDL_Texture* youLose = TextureManager::LoadTexture("../assets/lose.png");

            ScreenImage loser;
            loser.tex = youLose;
            s.x = 0; s.y = 100; d.x = d.y = 0;
            s.w = d.w = SCREEN_WIDTH; s.h = d.h = SCREEN_HEIGHT - 100;
            loser.src = s;
            loser.dest = d;

            d.y = 100;
            badEnd->AddComponent("Your Score: " + std::to_string(map->score - 1000), { 0, 0, 255 }, { 0, 0, 255 }, 60, d, false);

            badEnd->AddImage(loser); 
            badEnd->Render();

            SDL_Delay(4000);
            
            d.y = 600;
            badEnd->AddComponent("Play Again", { 0, 255, 255 }, { 255, 0, 255 }, 50, d, true);

            d.y = 660;
            badEnd->AddComponent("Exit", { 0, 255, 255 }, { 255, 0, 255 }, 50, d, true);

            badEnd->Render();

            while (gameState == BAD_END_SP) {
                while (SDL_PollEvent(&event) != 0) {
                    switch (event.type)
                    {
                        case SDL_QUIT:
                            gameState = EXITED;
                            isRunning = false;
                            break;
                        
                        default:
                            he_res = badEnd->HandleEvents (event);
                            if (he_res == 3) {
                                gameState = START_MENU;
                                map->lives_left = MAX_LIVES;
                            }
                            else if (he_res == 4) {
                                gameState = EXITED;
                                isRunning = false;
                            }
                            break;
                    }
                }
            }
        }

    }
}

void Game::handleEvents ()
{
    SDL_Event event;
    SDL_PollEvent(&event);
    switch (event.type) {
        case SDL_QUIT:
            isRunning = false;
            break;

        default:
            // std::cout << "Reached\n";
            map->UpdateMap(event);
            break;
    }
}

void Game::update ()
{
    // player->Update();
    // map->LoadMap();
    if (map->FrameCnt % FRAME_JUMP == 0)
    map->MoveMonsters();
}

void Game::CheckGoodEndGame ()
{
    int food_left = 0;
    for (int i = 0; i < 32; i++) {
        for (int j = 0; j < 32; j++) {
            if (map->map[i][j] == FOOD || map->map[i][j] == MONSTER1_ON_FOOD || map->map[i][j] == MONSTER2_ON_FOOD) {
                food_left++;
            }
        }
    }

    if (food_left == 0) {
        gameState = GOOD_END_SP;
        isRunning = false;
    }
}

void Game::CheckBadEndGame () 
{
    if ((map->Pacman->xmap == map->Monster1->xmap && map->Pacman->ymap == map->Monster1->ymap) || (map->Pacman->xmap == map->Monster2->xmap && map->Pacman->ymap == map->Monster2->ymap)) {
        if (map->lives_left == 0) {
            gameState = BAD_END_SP;
            isRunning = false;
        } else {
            map->lives_left--;
            SDL_Delay(2000);

            std::random_device dev;
            std::mt19937 rng(dev());
            std::uniform_int_distribution <std::mt19937::result_type> dist32(0,31);
            int pacmanX, pacmanY;
            pacmanX = dist32(rng); pacmanY = dist32(rng);
            while (map->map[pacmanY][pacmanX] == WALL || MD(pacmanX, pacmanY, map->Monster1->xmap, map->Monster1->ymap) < 10 || MD(pacmanX, pacmanY, map->Monster2->xmap, map->Monster2->ymap) < 10) {
                pacmanX = dist32(rng); pacmanY = dist32(rng);
            }

            map->map[map->Pacman->ymap][map->Pacman->xmap] = EMPTY;
            map->map[pacmanY][pacmanX] = PACMAN;
            map->Pacman->xmap = pacmanX; map->Pacman->ymap = pacmanY;
            map->Pacman->xpos = map->Pacman->xmap * TILE_WIDTH; map->Pacman->ypos = map->Pacman->ymap * TILE_HEIGHT;
            map->RenderMap ();
            SDL_Delay(2000);
        }
    }
}

void Game::UpdateScore () {
    map->score = 0;
    for (int i = 0; i < 32; i++) {
        for (int j = 0; j < 32; j++) {
            if (map->map[i][j] == EMPTY || map->map[i][j] == MONSTER1_ON_EMPTY || map->map[i][j] == MONSTER2_ON_EMPTY || map->map[i][j] == PACMAN) {
                map->score += 10;
            }
        }
    }
    map->score -= (map->FrameCnt / 6);
}

void Game::clean ()
{
    SDL_DestroyWindow(window);
    SDL_DestroyRenderer(renderer);
    SDL_Quit();
    std::cout << "Game Cleaned!\n";  
}