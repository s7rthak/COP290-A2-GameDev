#ifndef CONSTANTS_HPP
#define CONSTANTS_HPP

// Screen Dimensions
const int SCREEN_WIDTH = 960;
const int SCREEN_HEIGHT = 840;

// Level Dimensions
const int LEVEL_WIDTH = 960;
const int LEVEL_HEIGHT = 768;

// Tile constants
const int TILE_WIDTH = 30;
const int TILE_HEIGHT = 24;
const int TOTAL_TILES = 1024;

// Notation for map
const char WALL = 'w';
const char FOOD = 'f';
const char EMPTY = 'e';
const char PACMAN = 'p';
const char MONSTER1_ON_FOOD = 'u';
const char MONSTER1_ON_EMPTY = 'v';
const char MONSTER2_ON_FOOD = 'x';
const char MONSTER2_ON_EMPTY = 'y';

// Limiting Frame-rate
const int FPS = 50;
const int frameDelay = 1000 / FPS;

// Movement Velocities
const int MOVE_VEL_X = 5;
const int MOVE_VEL_Y = 4;

// Jump frames for monsters
const int FRAME_JUMP = 3;

// Monster AI difficulty
const int DIFFICULTY = 0;

// Screen Transitions.
const int SCREEN_SAME = 0;
const int SCREEN_REFRESH = 1;
const int SCREEN_CHANGE = 2;
const int SCREEN_CHANGE_SP = 3;
const int SCREEN_CHANGE_2P = 4;
const int SCREEN_CHANGE_BB = 5;

// Game States
const int EXITED = -1;
const int START_MENU = 0;
const int SINGLE_PLAYER = 1;
const int TWO_PLAYER = 2;
const int BLACK_BOX = 3;
const int GOOD_END_SP = 4;
const int BAD_END_SP = 5;

const int MAX_LIVES = 3;

#endif