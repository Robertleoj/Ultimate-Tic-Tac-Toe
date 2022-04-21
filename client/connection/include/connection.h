#include <static.h>
#include <helpers.h>

class Connection{
public:
    // Connection();
    bool init_connection(Turn &t);
    bool get_move(board_idx &mv);
    bool send_move(board_idx move);
    void close_connection();

private:
    SOCK_TYPE sock;
};