#include <bits/stdc++.h>
#include "constants.hpp"

using namespace std;

class Block {
    public:
        pair <int, int> center_cell;
        set <pair <int, int>> member_cells;
        int carried_over = -1;
        Block (int x, int y) {
            center_cell.first = x;
            center_cell.second = y;
            member_cells.insert(make_pair(x, y));
        }
};

int lie_position (int &x, int &a1, int &a2, int &a3, int &a4) {
    mt19937 gen;
    gen.seed(time(0));
    if (x < a1) {
        return 0;
    } else if (x > a1 && x < a2) {
        vector <int> w { x - a1, a2 - x };
        discrete_distribution <int> dist(w.begin(), w.end());
        if (dist(gen) == 0) return 0;
        else return 1;
    } else if (x > a2 && x < a3) {
        vector <int> w { x - a2, a3 - x };
        discrete_distribution <int> dist(w.begin(), w.end());
        if (dist(gen) == 0) return 1;
        else return 2;
    } else if (x > a3 && x < a4) {
        vector <int> w { x - a3, a4 - x };
        discrete_distribution <int> dist(w.begin(), w.end());
        if (dist(gen) == 0) return 2;
        else return 3;
    } else {
        return 3;
    }
}

int main () {
    vector <vector <bool>> grid(9, vector <bool> (5, false));
    vector <vector <int>> grid_member(9, vector <int> (5, -1));
    random_device dev;
    mt19937 rng(dev());
    uniform_int_distribution <mt19937::result_type> dist8(0,8);

    set <int> last_layer_centers;
    while (last_layer_centers.size() != 4) {
        last_layer_centers.insert(dist8(rng));
    }

    vector <int> selection;
    vector <Block> blocks;
    for (auto it = last_layer_centers.begin(); it != last_layer_centers.end(); ++it) {
        selection.push_back(*it);
        grid[*it][4] = true;
        grid_member[*it][4] = blocks.size();
        Block tmp (4, *it);
        blocks.push_back(tmp);
    }

    for (int i = 0; i < 9; i++) {
        if (!grid[i][4]) {
            int center_chosen = lie_position (i, selection[0], selection[1], selection[2], selection[3]);
            for (int j = min(selection[center_chosen], i); j <= max(selection[center_chosen], i); j++) {
                if (!grid[j][4]) {
                    grid[j][4] = true;
                    grid_member[j][4] = center_chosen;
                    blocks[center_chosen].member_cells.insert(make_pair(4, i));
                }
            }
        }
    }

    // Assigning probabilistic weights for block-expansion based on block-size.
    vector <int> prob_based_on_size(6, 0);
    prob_based_on_size[0] = 5;
    prob_based_on_size[1] = 100;
    prob_based_on_size[2] = 25;
    prob_based_on_size[3] = 5;
    prob_based_on_size[4] = 1;
    prob_based_on_size[5] = 0;
    
    for (int k = 3; k > 0; k--) {
        for (int i = 0; i < 9; i++) {
            if (i != 0) {
                if (blocks[grid_member[i-1][k]].carried_over == -1 && blocks[grid_member[i][k+1]].carried_over == -1) {
                    random_device rd;
                    mt19937 g(rd());
                    vector <int> w {prob_based_on_size[0], prob_based_on_size[blocks[grid_member[i][k+1]].member_cells.size()], prob_based_on_size[blocks[grid_member[i-1][k]].member_cells.size()]};
                    discrete_distribution <int> dist(w.begin(), w.end());
                    int x = dist(g);
                    if (x == 0) {
                        Block tmp (i, k);
                        grid_member[i][k] = blocks.size();
                        blocks.push_back(tmp);
                    } else if (x == 1) {
                        if (blocks[grid_member[i][k+1]].member_cells.size() > 1) blocks[grid_member[i][k+1]].carried_over = i;
                        blocks[grid_member[i][k+1]].member_cells.insert(make_pair(i, k));
                        grid_member[i][k] = grid_member[i][k+1];
                    } else {
                        blocks[grid_member[i-1][k]].member_cells.insert(make_pair(i, k));
                        grid_member[i][k] = grid_member[i-1][k];
                    }
                } else if (blocks[grid_member[i][k+1]].carried_over == i) {
                    random_device rd;
                    mt19937 g(rd());
                    vector <int> w {prob_based_on_size[0], prob_based_on_size[blocks[grid_member[i][k+1]].member_cells.size()]};
                    discrete_distribution <int> dist(w.begin(), w.end());
                    if (dist(g) == 0) {
                        Block tmp (i, k);
                        grid_member[i][k] = blocks.size();
                        blocks.push_back(tmp);
                    } else {
                        if (blocks[grid_member[i][k+1]].member_cells.size() > 1) blocks[grid_member[i][k+1]].carried_over = true;
                        blocks[grid_member[i][k+1]].member_cells.insert(make_pair(i, k));
                        grid_member[i][k] = grid_member[i][k+1];
                    }
                } else {
                    Block tmp (i, k);
                    grid_member[i][k] = blocks.size();
                    blocks.push_back(tmp);
                }
            } else {
                random_device rd;
                mt19937 g(rd());
                vector <int> w {prob_based_on_size[0], prob_based_on_size[blocks[grid_member[i][k+1]].member_cells.size()]};
                discrete_distribution <int> dist(w.begin(), w.end());
                if (dist(g) == 0) {
                    Block tmp (i, k);
                    grid_member[i][k] = blocks.size();
                    blocks.push_back(tmp);
                } else {
                    if (blocks[grid_member[i][k+1]].member_cells.size() > 1) blocks[grid_member[i][k+1]].carried_over = i;
                    blocks[grid_member[i][k+1]].member_cells.insert(make_pair(i, k));
                    grid_member[i][k] = grid_member[i][k+1];
                }
            }
            grid[i][k] = true;
        }
    }

    for (int i = 0; i < 9; i++) {
        if (i != 0 && grid_member[i-1][0] != -1 && !blocks[grid_member[i-1][0]].carried_over) {
            random_device rd;
            mt19937 g(rd());
            vector <int> w {prob_based_on_size[0], prob_based_on_size[blocks[grid_member[i][1]].member_cells.size()], prob_based_on_size[blocks[grid_member[i-1][0]].member_cells.size()]};
            discrete_distribution <int> dist(w.begin(), w.end());
            int x = dist(g);
            if (x == 0) {} 
            else if (x == 1) {
                blocks[grid_member[i][1]].member_cells.insert(make_pair(i, 0));
                grid_member[i][0] = grid_member[i][1];
            } else {
                blocks[grid_member[i-1][0]].member_cells.insert(make_pair(i, 0));
                grid_member[i][0] = grid_member[i-1][0];
            }
        } else {
            random_device rd;
            mt19937 g(rd());
            vector <int> w {prob_based_on_size[0], prob_based_on_size[blocks[grid_member[i][1]].member_cells.size()]};
            discrete_distribution <int> dist(w.begin(), w.end());
            if (dist(g) == 0) {} 
            else {
                blocks[grid_member[i][1]].member_cells.insert(make_pair(i, 0));
                grid_member[i][0] = grid_member[i][1];
            }
        }
        grid[i][0] = true;
    }


    // discrete_distribution <int> test{1, 2, 1};
    // cout << test(gen) << "\n";

    vector <vector <int>> pos_paths (9, vector <int> (9, -1));
    map <int, int> symmetry;
    int cnt = blocks.size();

    for (int i = 0; i < 9; i++) {
        for (int j = 0; j < 5; j++) {
            pos_paths[i][j] = grid_member[i][j];
        }
    }

    for (int i = 0; i < 9; i++) {
        for (int j = 5; j < 9; j++) {
            pos_paths[i][j] = grid_member[i][8-j];
        }
    }

    vector <vector <char>> final_grid (32, vector <char> (32, WALL));
    for (int i = 0; i < 9; i++) {
        for (int j = 0; j < 9; j++) {
            if (pos_paths[i][j] != -1) {
                int x = 3*i+2, y = 3*j+2;
                final_grid[x][y] = FOOD;
                final_grid[x+3][y] = FOOD;
                final_grid[x+3][y+3] = FOOD;
                final_grid[x][y+3] = FOOD;
                if (j - 1 < 0 || pos_paths[i][j-1] != pos_paths[i][j]) {
                    final_grid[x+1][y] = FOOD;
                    final_grid[x+2][y] = FOOD;
                }
                if (j + 1 > 8 || pos_paths[i][j+1] != pos_paths[i][j]) {
                    final_grid[x+1][y+3] = FOOD;
                    final_grid[x+2][y+3] = FOOD;
                }
                if (i - 1 < 0 || pos_paths[i-1][j] != pos_paths[i][j]) {
                    final_grid[x][y+1] = FOOD;
                    final_grid[x][y+2] = FOOD;
                }
                if (i + 1 > 8 || pos_paths[i+1][j] != pos_paths[i][j]) {
                    final_grid[x+3][y+1] = FOOD;
                    final_grid[x+3][y+2] = FOOD;
                }
            }
        }
    }

    // Remove false-positives
    for (int i = 0; i < 32; i++) {
        for (int j = 0; j < 32; j++) {
            if (final_grid[i][j] == FOOD) {
                if (final_grid[i-1][j] == WALL && final_grid[i+1][j] == WALL && final_grid[i][j-1] == WALL && final_grid[i][j+1] == WALL) {
                    final_grid[i][j] = WALL;
                }
            }
        }
    }

    // Random initialisation of pacman co-ordinates.
    uniform_int_distribution <mt19937::result_type> dist32(0,31);
    int pacmanX, pacmanY;
    pacmanX = dist32(rng); pacmanY = dist32(rng);
    while (final_grid[pacmanX][pacmanY] == WALL) {
        pacmanX = dist32(rng); pacmanY = dist32(rng);
    }

    // Random monster(ghost) position initialisation such that MD >= 10.
    int monster1X, monster1Y;
    monster1X = dist32(rng); monster1Y = dist32(rng);
    while (final_grid[monster1X][monster1Y] == WALL || abs(monster1X - pacmanX) + abs(monster1Y - pacmanY) < 10) {
        monster1X = dist32(rng); monster1Y = dist32(rng);
    }

    int monster2X, monster2Y;
    monster2X = dist32(rng); monster2Y = dist32(rng);
    while (final_grid[monster2X][monster2Y] == WALL || abs(monster2X - pacmanX) + abs(monster2Y - pacmanY) < 10) {
        monster2X = dist32(rng); monster2Y = dist32(rng);
    }

    // Change map positions likewise.
    final_grid[pacmanX][pacmanY] = PACMAN;
    final_grid[monster1X][monster1Y] = MONSTER1_ON_FOOD;
    final_grid[monster2X][monster2Y] = MONSTER2_ON_FOOD;

    // for (int i = 0; i < 32; i++) {
    //     for (int j = 0; j < 32; j++) {
    //         cout << final_grid[i][j];
    //     }
    //     cout << "\n";
    // }

    fstream file("../assets/maze_layout.txt", fstream::out | fstream::trunc);
    for (int i = 0; i < 32; i++) {
        for (int j = 0; j < 32; j++) {
            file << final_grid[i][j] << " ";
        }
        file << "\n";
    }
    file.close();
}