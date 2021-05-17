#include "two_player_map.hpp"
#include "texture_manager.hpp"
#include "constants.hpp"
#include "util.hpp"
#include "game.hpp"
#include "map_layout_read.hpp"
#include <boost/array.hpp>
#include <SDL2/SDL_ttf.h>

using namespace boost::asio;
using ip::udp;

std::vector <std::string> split (std::string &text) {
    std::istringstream iss(text);
    std::string s;
    std::vector <std::string> res;

    while (getline(iss, s, ' ')) {
        res.push_back(s);
    }

    return res;
}

std::string recv_string (udp::socket &socket, boost::array<char, 128> &buff,udp::endpoint &sender) {
    std::string recv_mess;
    buff.assign(0);
    socket.receive_from(buffer(buff), sender);
    recv_mess = buff.data();
    while (recv_mess == "Keep Alive") {
        buff.assign(0);
        socket.receive_from(buffer(buff), sender);
        recv_mess = buff.data();
    }
    return recv_mess;
}

TwoPlayerMap::TwoPlayerMap (char** lvl) 
{
    std::random_device dev;
    std::mt19937 rng(dev());

    std::uniform_int_distribution<std::mt19937::result_type> dist(1235,50000);
    int my_port = dist(rng);

    ///////////////////////////////////////////////////////////////////////////////////////////////////////////////
    io_service io_service;
    
    //socket creation
    udp::socket socket(io_service);
    
    //connection
    socket.open(udp::v4());
    udp::endpoint endpoint_ = udp::endpoint{ip::address::from_string("127.0.0.1"), my_port};
    my_endpoint = &endpoint_;
    socket.bind(*my_endpoint);

    const int timeout = 200;
    // ::setsockopt(socket.native_handle(), SOL_SOCKET, SO_RCVTIMEO, (const char *)&timeout, sizeof timeout);//SO_SNDTIMEO for send ops
    
    // request/message from client
    const std::string msg = "Hello from Client!\n";
    udp::endpoint server = udp::endpoint{ip::address::from_string("127.0.0.1"), 1234};
    socket.send_to(buffer("Hi!"), server);
    std::cout << "Client sent hello message!" << std::endl;
    
    // getting response from server
    boost::array<char, 128> recv_buf;
    size_t len = socket.receive_from(buffer(recv_buf), server);
    
    std::string message = recv_buf.data();
    std::cout << message << "\n";

    std::vector <std::string> peer_info = split(message);
    udp::endpoint peer = udp::endpoint{ip::address::from_string(peer_info[0]), stoi(peer_info[1])};

    socket.send_to(buffer("Hello peer"), peer);
    socket.receive_from(buffer(recv_buf), peer);
    socket.receive_from(buffer(recv_buf), server);

    /////////////////////////////////////////////////////////////////////////////////////////////////////////////////

    my_socket = &socket;
    opponent_endpoint = &peer;
    opponent_lives_left = MAX_LIVES;
    opponent_last_frame = -1;
    opponent_cur_frame = -1;
    opponent_score = 0;

    std::uniform_int_distribution<std::mt19937::result_type> dist7(1,7);
    int my_value = dist7(rng), opponent_value;
    std::string opp_mess;

    assert(peer == *opponent_endpoint);

    my_socket->send_to(buffer(std::to_string(my_value)), *opponent_endpoint);
    opp_mess = recv_string(*my_socket, recv_buf, *opponent_endpoint);
    opponent_value = std::stoi(opp_mess);
    std::cout << my_value << " " << opponent_value << "\n";
    while (my_value == opponent_value) {
        my_value = dist7(rng);
        my_socket->send_to(buffer(std::to_string(my_value)), *opponent_endpoint);
        opp_mess = recv_string(*my_socket, recv_buf, *opponent_endpoint);
        opponent_value = std::stoi(opp_mess);
    }

    isHost = my_value > opponent_value;
    int map_chosen;
    std::vector <int> posi;
    if (isHost) {
        map_chosen = dist7(rng);
        my_socket->send_to(buffer(std::to_string(map_chosen)), *opponent_endpoint);
    } else {
        opp_mess = recv_string(*my_socket, recv_buf, *opponent_endpoint);
        map_chosen = std::stoi(opp_mess);
    }

    // Initialize Game Resources.
    std::cout << "Map Chosen: " << map_chosen << "\n";
    map = lvl;
    posi = ReadLayout(map, "../assets/Multiplayer/map_" + std::to_string(map_chosen) + ".txt");
    lives_left = MAX_LIVES;
    // assert(posi.size() == 6);

    std::string WallChosen = "../assets/wall-" + std::to_string(TWO_PLAYER_LAYOUT_COLOR) + ".png";

    wall = TextureManager::LoadTexture(WallChosen.c_str());
    food = TextureManager::LoadTexture("../assets/food.png");
    pacman_fo = TextureManager::LoadTexture("../assets/pacman_full_open.png");
    pacman_ho = TextureManager::LoadTexture("../assets/pacman_half_open.png");
    pacman_c = TextureManager::LoadTexture("../assets/pacman_close.png");
    monster1 = TextureManager::LoadTexture("../assets/monster1.png");
    monster2 = TextureManager::LoadTexture("../assets/monster2.png");

    src.x = src.y = 0;
    src.w = dest.w = TILE_WIDTH;
    src.h = dest.h = TILE_HEIGHT;

    Pacman = new GameObject(posi[1], posi[0], map);
    Monster1 = new GameObject(posi[3], posi[2], map);
    Monster2 = new GameObject(posi[5], posi[4], map);

}

void TwoPlayerMap::UpdateMap (SDL_Event &e) 
{
    Pacman->handleEvent (e);
    Pacman->Update ();
}

void TwoPlayerMap::MoveMonsters ()
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

void TwoPlayerMap::DrawMap () 
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

void TwoPlayerMap::RenderMap ()
{
    SDL_Rect s, d;
    SDL_RenderClear(Game::renderer);

    DrawMap();

    TTF_Font* font = TTF_OpenFont("../assets/EvilEmpire-4BBVK.ttf", 50);

    // Set-up score-board texture.
    std::string text = "Score: ";
    if (isHost) text += std::to_string(score);
    else text += std::to_string(opponent_score);
    SDL_Surface* textSurface = TTF_RenderText_Solid(font, text.c_str(), { 0, 255, 255 });
    SDL_Texture* score_board = SDL_CreateTextureFromSurface(Game::renderer, textSurface);
    SDL_FreeSurface(textSurface);

    int w, h;
    SDL_QueryTexture(score_board, NULL, NULL, &w, &h);

    s.x = s.y = 0; d.x = 20; d.y = 780; s.w = d.w = w; s.h = d.h = h;
    TextureManager::Draw(score_board, s, d, 0.0f, false, 0);

    // Warn player in case of weak connection.
    if (opponent_cur_frame - opponent_last_frame > WEAK_CONNECTION) {
        std::string wc = "Weak Connection";
        textSurface = TTF_RenderText_Shaded(font, wc.c_str(), { 255, 0, 0 }, { 0, 255, 0 });
        SDL_Texture* weak_con = SDL_CreateTextureFromSurface(Game::renderer, textSurface);
        SDL_FreeSurface(textSurface);

        SDL_QueryTexture(weak_con, NULL, NULL, &w, &h);
        s.x = s.y = 0; d.x = (SCREEN_WIDTH - w) / 2; d.y = 360; s.w = d.w = w; s.h = d.h = h;
        TextureManager::Draw(weak_con, s, d, 0.0f, false, 0);
    }

    TTF_CloseFont(font);

    // Set-up Lives texture.
    SDL_Texture* life = TextureManager::LoadTexture("../assets/life.png");
    SDL_QueryTexture(life, NULL, NULL, &w, &h);

    s.x = s.y = 0; s.w = d.w = w; s.h = d.h = h; d.y = 780;
    for (int i = 0; (isHost && i < lives_left) || (!isHost && i < opponent_lives_left); i++) {
        d.x = SCREEN_WIDTH - (i + 1) * (w - 3);
        TextureManager::Draw(life, s, d, 0.0f, false, 0); 
    }

    SDL_RenderPresent(Game::renderer);
}

void TwoPlayerMap::ExchangeMapInfo ()
{
    // Socket refresh and exchange info.
    io_service io_service;
    udp::socket socket(io_service);
    socket.open(udp::v4());
    socket.bind(*my_endpoint);
    boost::array<char, 128> recv_buf;

    if (isHost) {
        socket.send_to(buffer(std::to_string(FrameCnt) + " " + std::to_string(score) + " " + std::to_string(lives_left)), *opponent_endpoint);
    } else {
        std::string opp_info = recv_string(socket, recv_buf, *opponent_endpoint);
        std::vector <std::string> all_info = split(opp_info);
        opponent_last_frame = opponent_cur_frame;
        opponent_cur_frame = std::stoi(all_info[0]);
        opponent_score = std::stoi(all_info[1]);
        opponent_lives_left = std::stoi(all_info[2]);
    }
}