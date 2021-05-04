#ifndef UTIL_HPP
#define UTIL_HPP

#include <bits/stdc++.h>
#include "game_object.hpp"
#include "constants.hpp"

class FrontierEntry {
    public:
        int x;
        int y;

        int h, f, g;

        std::string first_action;

        FrontierEntry (int xp, int yp, int g_value, int h_value, std::string action) {
            x = xp;
            y = yp;
            g = g_value;
            h = h_value;
            f = g + h;
            first_action = action;
        }
};
bool operator<(const FrontierEntry& f1, const FrontierEntry& f2) {
    return f2.f < f1.f;
}


int ManhattanDistance (int x1, int y1, int x2, int y2) {
    return abs(x1 - x2) + abs(y1 - y2);
}

int power (int a, int b) {
    if (b == 0) return 1;
    if (b == 1) return a;
    
    int tmp = power(a, b/2);
    
    if (b%2 == 0) return tmp * tmp;
    else return tmp * tmp * a;
}

bool MonsterInfested (GameObject* mon2, int xp, int yp) {
    if (ManhattanDistance(mon2->ymap, mon2->xmap, xp, yp) < 3) return true;
    return false;
}

bool WallChecker (GameObject* mon1, GameObject* mon2, std::string dir) {
    if (mon1->xpos % TILE_WIDTH != 0) {
        if (dir == "LEFT" || dir == "RIGHT") return true; 
        else return false;
    }
    else if (mon1->ypos % TILE_HEIGHT != 0) {
        if (dir == "UP" || dir == "DOWN") return true;
        else return false;
    }
    else {
        if (dir == "UP") {
            if (mon1->map[mon1->ymap - 1][mon1->xmap] == WALL || MonsterInfested(mon2, mon1->ymap - 1, mon1->xmap)) return false;
            else return true;
        }
        else if (dir == "DOWN") {
            if (mon1->map[mon1->ymap + 1][mon1->xmap] == WALL || MonsterInfested(mon2, mon1->ymap + 1, mon1->xmap)) return false;
            else return true;
        }
        else if (dir == "LEFT") {
            if (mon1->map[mon1->ymap][mon1->xmap - 1] == WALL || MonsterInfested(mon2, mon1->ymap, mon1->xmap - 1)) return false;
            else return true;
        }
        else if (dir == "RIGHT") {
            if (mon1->map[mon1->ymap][mon1->xmap + 1] == WALL || MonsterInfested(mon2, mon1->ymap, mon1->xmap + 1)) return false;
            else return true;
        }
    }
    return false;
}
 
std::pair <int,std::string> ChooseGreedyActionMD (GameObject* mon1, GameObject* mon2, GameObject* pacman, int difficulty, int frame_jump, int left, std::string last_action) {
    if (left == 0) {
        std::vector <std::string> pos_actions;

        if (WallChecker(mon1, mon2, "UP")) {
            pos_actions.push_back("UP");
        }
        if (WallChecker(mon1, mon2, "DOWN")) {
            pos_actions.push_back("DOWN");
        }
        if (WallChecker(mon1, mon2, "RIGHT")) {
            pos_actions.push_back("RIGHT");
        }
        if (WallChecker(mon1, mon2, "LEFT")) {
            pos_actions.push_back("LEFT");
        }

        assert(pos_actions.size() != 0);

        std::random_device rd;
        std::mt19937 g(rd());
        std::vector <int> w {1, power(10, difficulty)};
        std::discrete_distribution <int> d (w.begin(), w.end());

        int x = d(g);

        if (x == 0) {
            std::uniform_int_distribution <int> dist(0, pos_actions.size()-1);
            int choice = dist(g);
            return make_pair(std::max(6 / frame_jump, 1) - 1, pos_actions[choice]);
            // return make_pair(0, pos_actions[choice]);
        }
        else {
            std::vector <int> md;
            for (int i = 0; i < pos_actions.size(); i++) {
                if (pos_actions[i] == "UP") {
                    md.push_back(ManhattanDistance(mon1->xmap, mon1->ymap - std::max(1, frame_jump / 6), pacman->xmap, pacman->ymap));
                }
                else if (pos_actions[i] == "DOWN") {
                    md.push_back(ManhattanDistance(mon1->xmap, mon1->ymap + std::max(1, frame_jump / 6), pacman->xmap, pacman->ymap));
                }
                else if (pos_actions[i] == "RIGHT") {
                    md.push_back(ManhattanDistance(mon1->xmap + std::max(1, frame_jump / 6), mon1->ymap, pacman->xmap, pacman->ymap));
                }
                else if (pos_actions[i] == "LEFT") {
                    md.push_back(ManhattanDistance(mon1->xmap - std::max(1, frame_jump / 6), mon1->ymap, pacman->xmap, pacman->ymap));
                }
            }

            int min_index = 0;
            for (int i = 0; i < pos_actions.size(); i++) {
                if (md[i] < md[min_index]) min_index = i;
            }

            // for (int i = 0; i < pos_actions.size(); i++) {
            //     std::cout << pos_actions[i] << " ";
            // }
            // std::cout << "\n";

            return make_pair(std::max(6 / frame_jump, 1) - 1, pos_actions[min_index]);
            // return make_pair(0, pos_actions[min_index]);
        }
    }
    else {
        return make_pair(left - 1, last_action);
    }
}

// std::pair <int, std::string> ChooseGreedyActionAStar (GameObject* monster, GameObject* pacman, int difficulty, int frame_jump, int left, std::string last_action) {
//     if (left == 0) {
//         std::vector <std::string> pos_actions;

//         if (WallChecker(monster, monster->xpos, monster->ypos - frame_jump * MOVE_VEL_Y, "UP")) {
//             pos_actions.push_back("UP");
//         }
//         if (WallChecker(monster, monster->xpos, monster->ypos + frame_jump * MOVE_VEL_Y, "DOWN")) {
//             pos_actions.push_back("DOWN");
//         }
//         if (WallChecker(monster, monster->xpos + frame_jump * MOVE_VEL_X, monster->ypos, "RIGHT")) {
//             pos_actions.push_back("RIGHT");
//         }
//         if (WallChecker(monster, monster->xpos - frame_jump * MOVE_VEL_X, monster->ypos, "LEFT")) {
//             pos_actions.push_back("LEFT");
//         }

//         assert(pos_actions.size() != 0);

//         std::random_device rd;
//         std::mt19937 g(rd());
//         std::vector <int> w {1, power(10, difficulty)};
//         std::discrete_distribution <int> d (w.begin(), w.end());

//         int x = d(g);

//         if (x == 0) {
//             std::uniform_int_distribution <int> dist(0, pos_actions.size()-1);
//             int choice = dist(g);
//             return make_pair(std::max(6 / frame_jump, 1) - 1, pos_actions[choice]);
//             // return make_pair(0, pos_actions[choice]);
//         }
//         else {
//             std::priority_queue <FrontierEntry> frontier;
//         }
//     }
//     else {
//         return make_pair(left - 1, last_action);
//     }
// }


#endif