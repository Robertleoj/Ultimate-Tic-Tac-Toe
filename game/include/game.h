#pragma once
#include <state.h>
#include <static.h>

class Game{
public:

    Game();

    ~Game();

    StateData get_state_data();

    void move(board_idx move);

private:
    State * state;
};
