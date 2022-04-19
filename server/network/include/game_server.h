#include <helpers.h>
#include <game.h>
#include <memory>

class GameServer{

public:
    GameServer(std::pair<SOCK_TYPE, SOCK_TYPE> player_sockets);

    void play();

private:
    std::pair<SOCK_TYPE, SOCK_TYPE> player_sockets;
    board_idx last_move;

    void send_move_to(
        SOCK_TYPE player_sock, 
        int *move
    );

    void get_move(SOCK_TYPE player_sock, int move_buffer[]);
    void send_who();
    std::unique_ptr<Game> game;
};