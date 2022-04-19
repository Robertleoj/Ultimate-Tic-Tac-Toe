#include <iostream>
#include <helpers.h>
#include <vector>
#include <game_server.h>



void main_loop(SOCK_TYPE &sock){
    int num_players = 0;

    std::vector<SOCK_TYPE> player_sockets;
    //std::vector<Game> games;

    while(num_players < 2){
        SOCK_TYPE new_sock;
        sockaddr client_addr;
        if(!accept_connection(new_sock, sock, &client_addr)){
            std::cout << "accept() failed" << std::endl;
            continue;
        }
        ushort client_port;
        std::string client_ip;
        get_port_and_ip(&client_addr, &client_port, client_ip);
        std::cout << "Client connected with port " << client_port
                  << " and ip " << client_ip
                  << std::endl;
        player_sockets.push_back(new_sock);
        num_players++;
        if(num_players == 2){
            GameServer game = GameServer(
                std::make_pair(player_sockets[0], player_sockets[1])
            );
            // TODO: put on thread
            std::cout << "Let the game begin " << std::endl;
            game.play();
            player_sockets.clear();
        }
    }
}

int main(int argc, char *argv[]){

    ushort port = DEFAULT_PORT;
    sockaddr socket_address;

    SOCK_TYPE sock = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);
    int optval = 1;
    setsockopt(sock, SOL_SOCKET, SO_REUSEADDR, &optval, sizeof(optval));

    make_sock_addr(&socket_address, port);
    if(!bind_sock_to_addr(sock, &socket_address)){
        exit(0);
    }

    if(!listen_at_sock(sock)){
        exit(0);
    }

    std::cout << "Listening on port " << port << std::endl;

    main_loop(sock);

}

