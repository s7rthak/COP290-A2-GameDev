#ifndef UTIL_HPP
#define UTIL_HPP

#include <bits/stdc++.h>
#include "game_object.hpp"

// class FrontierEntry {
//     public:
//         int x;
//         int y;

//         int h, f, g;

//         std::string first_action;

//         FrontierEntry (int xp, int yp, int g_value, int h_value, std::string action) {
//             x = xp;
//             y = yp;
//             g = g_value;
//             h = h_value;
//             f = g + h;
//             first_action = action;
//         }
// };
// bool operator<(const FrontierEntry& f1, const FrontierEntry& f2) {
//     return f2.f < f1.f;
// }


extern int ManhattanDistance (int x1, int y1, int x2, int y2);

extern int power (int a, int b);

extern bool MonsterInfested (GameObject* mon2, int xp, int yp);

extern bool WallChecker (GameObject* mon1, GameObject* mon2, std::string dir);
 
extern std::pair <int,std::string> ChooseGreedyActionMD (GameObject* mon1, GameObject* mon2, GameObject* pacman, int difficulty, int frame_jump, int left, std::string last_action);

// extern std::pair <int, std::string> ChooseGreedyActionAStar (GameObject* monster, GameObject* pacman, int difficulty, int frame_jump, int left, std::string last_action);

#endif