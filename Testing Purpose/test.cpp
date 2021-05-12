#include <SDL2/SDL_net.h>
#include <bits/stdc++.h>

int main () { 
    SDLNet_Init();
    UDPsocket sd = SDLNet_UDP_Open(23000);
    UDPpacket* p;
    int quit; 

    IPaddress* myaddress = SDLNet_UDP_GetPeerAddress(sd, -1);
    p = SDLNet_AllocPacket(1024);

    quit = 0;
    while (!quit) {
        if (SDLNet_UDP_Recv(sd, p)) {
            std::cout << "UDP Packet received" << "\n";
        }
        if (!strcmp((char *)p->data, "quit")) quit = 1;
    }
    SDLNet_FreePacket(p);
    SDLNet_Quit();
}