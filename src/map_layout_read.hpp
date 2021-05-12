#ifndef MAP_LAYOUT_READ_HPP
#define MAP_LAYOUT_READ_HPP

#include <bits/stdc++.h>
#include "constants.hpp"

std::vector<int> ReadLayout (char** lvl_rand, std::string layout) {
    std::vector <int> positions (6);

    std::ifstream ReadFile (layout);
    std::string ch;

    for (int i = 0; i < 32; i++) {
        for (int j = 0; j < 32; j++) {
            ReadFile >> ch;
            lvl_rand[i][j] = ch[0];
            if (ch[0] == PACMAN) {
                positions[0] = i;
                positions[1] = j;
            } 
            else if (ch[0] == MONSTER1_ON_EMPTY || ch[0] == MONSTER1_ON_FOOD) {
                positions[2] = i;
                positions[3] = j;
            } 
            else if (ch[0] == MONSTER2_ON_EMPTY || ch[0] == MONSTER2_ON_FOOD) {
                positions[4] = i;
                positions[5] = j;
            }
        }
    }

    for (int i = 0; i < 32; i++) {
        for (int j = 0; j < 32; j++) {
            std::cout << lvl_rand[i][j];
        }
        std::cout << "\n";
    }

    return positions;
}

#endif