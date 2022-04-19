#include <static.h>
#include <helpers.h>

class Connection{
public:
    // Connection();
    Turn init_connection();
    board_idx get_move();
    void send_move(board_idx move);

private:
    SOCK_TYPE sock;
};