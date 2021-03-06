#ifndef MAP_HPP
#define MAP_HPP

#include "game_object.hpp"
#include <bits/stdc++.h>

class Map {
    public:
        Map (char** lvl, std::vector<int> &posi);
        ~Map ();

        void LoadMap (char** lvl);
        void UpdateMap (SDL_Event &e);
        void MoveMonsters ();
        void DrawMap ();
        void RenderMap ();
        char** map;
        int FrameCnt = 0;
        GameObject* Pacman;
        GameObject* Monster1;
        GameObject* Monster2;        

        int tmp1 = 0;
        int tmp2 = 0;
        std::string lma1;
        std::string lma2;

        int score = 0;
        int lives_left;

    private:
        SDL_Rect src, dest;

        SDL_Texture* wall;
        SDL_Texture* food;
        SDL_Texture* pacman_fo;
        SDL_Texture* pacman_ho;
        SDL_Texture* pacman_c;
        SDL_Texture* monster1;
        SDL_Texture* monster2;

};

#endif