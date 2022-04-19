#include <game_server.h>
#include <static.h>
#include <iostream>
#include <static.h>

GameServer::GameServer(std::pair<SOCK_TYPE, SOCK_TYPE> player_sockets){
    this->player_sockets = player_sockets;
    this->game = std::unique_ptr<Game>(new Game());
}

void GameServer::send_move_to(
    SOCK_TYPE player_sock, int *move
) {

    if(send(player_sock, (char *) move, sizeof(int) * 4, 0) < 0){
        // TODO: Connection issue, try again??
        std::cout << "send() failed" << std::endl;
        exit(0);
    }
}

void GameServer::get_move(SOCK_TYPE player_sock, int move_buffer[]){

    int buffsize = 100;
    char recv_buffer[buffsize];
    // We want to receive four integers
    std::cout << __LINE__ << std::endl;
    int amount_to_receive = sizeof(int) * 4;
    int total_received = 0;
    int n;
    std::cout << __LINE__ << std::endl;
    while(total_received < amount_to_receive){
        n = recv(
            player_sock, recv_buffer + total_received, 
            buffsize - total_received, 0
        );

        if(n <= 0){
            std::cout << "failed to recv() move" << std::endl;
            exit(0);
        }
        total_received += n;
    }
    std::cout << __LINE__ << std::endl;

    
    for(int i = 0; i < 4; i++){
        int coord = *((int *)(&recv_buffer[i * sizeof(int)]));
        move_buffer[i] = coord;
    }
}

void GameServer::send_who(){
    bool b = true;
    if(send(this->player_sockets.first, (char*)&b, 1, 0) < 0){
        // TODO: Connection issue, try again??
        std::cout << "send() failed" << std::endl;
        exit(0);
    }
    b = false;
    if(send(this->player_sockets.second, (char*)&b, 1, 0) < 0){
        // TODO: Connection issue, try again??
        std::cout << "send() failed" << std::endl;
        exit(0);
    }
}

void GameServer::play(){
    StateData state_data = this->game->get_state_data();
    send_who();
    int move_buffer[4];
    while(!state_data.terminal.first){
        board_idx move;
        if(state_data.turn == CROSS_TURN){
            get_move(this->player_sockets.first, move_buffer);
            send_move_to(this->player_sockets.second, move_buffer);
        } else {
            get_move(this->player_sockets.second, move_buffer);
            send_move_to(this->player_sockets.first, move_buffer);
        }
        for(int i = 0; i < 4; i++){
            move.push_back(move_buffer[i]);
        }
        std::cout << __LINE__ << std::endl;
        this->game->move(move);
        state_data = this->game->get_state_data();
        std::cout << __LINE__ << std::endl;
        // send_state_data(&state_data);
    }
}