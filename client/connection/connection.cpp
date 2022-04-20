#include <connection.h>
#include <iostream>


Turn Connection::init_connection(){
    // make the socket
    this->sock = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);
    if(this->sock < 0){
        std::cout << "socket() failed" << std::endl;
    }
    
    if(!connect_to_server(sock, DEFAULT_PORT, LOCALHOST)){
        // exit(0);
    }

    // get whose turn it is - it is a boolean
    char buff[100];
    int n = recv(this->sock, buff, 1, 0);
    if(n <= 0){
        std::cout << "failed to recv() who we are" << std::endl;
    }

    bool turn = *((bool *) buff);

    return turn ? CROSS_TURN : CIRCLE_TURN;
}

/*
Send a move to the server
returns true on success, false otherwise
*/
bool Connection::send_move(board_idx move){
    int move_buff[4];
    for(int i = 0; i < 4; i++){
        move_buff[i] = move[i];
    }

    if(send(this->sock, (char *) move_buff, sizeof(int) * 4, 0) < 0){
        std::cout << "failed to send() move" << std::endl;
        return false;
    }
    return true;
}

/*
Receive the opponent move from the server
Will replace the move variable with the received move
returns true on success, false otherwise
*/
bool Connection::get_move(board_idx &move){
    char move_buff[sizeof(int) * 4];

    int total_received = 0;
    int goal_receive = sizeof(int) * 4;
    int n;
    while(total_received < goal_receive){
        n = recv(
            this->sock, move_buff + total_received, 
            sizeof(int) * 4 - total_received,0 
        );
        if(n < 0){
            std::cout << "failed to recv() move" << std::endl;
            return false;
        }
        total_received += n;
    }
    board_idx mv;
    int * move_arr = (int *) move_buff;
    for(int i = 0; i < 4; i++){
        move.push_back(move_arr[i]);
    }

    move = mv;
    return true;
}