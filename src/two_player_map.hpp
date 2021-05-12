#ifndef TWO_PLAYER_HPP
#define TWO_PLAYER_HPP

#include "game_object.hpp"
#include <boost/asio.hpp>

using namespace boost::asio;
using ip::udp;

class TwoPlayerMap {
    public:
        TwoPlayerMap ();
        ~TwoPlayerMap ();

        void UpdateMap (SDL_Event &e);
        void MoveMonsters ();
        void DrawMap ();
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
        bool isHost;
        bool gameWon = false;

        udp::socket* my_socket;
        // Other player info
        udp::endpoint* opponent_endpoint;
        int opponent_score;
        int opponent_lives_left;
        int opponent_last_frame;

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