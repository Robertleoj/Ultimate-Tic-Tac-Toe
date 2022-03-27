#pragma once
#include<vector>

typedef std::vector<int> board_idx;

enum Condition
{
    EMPTY,
    CROSS,
    CIRCLE,
    DRAW
};

enum Turn {
    CROSS_TURN,
    CIRCLE_TURN
};



struct StateData{
    Condition ****board;
    Condition **win_boards;
    Turn turn;
    std::vector<board_idx> allowed_moves;
    std::pair<int, int> active_board; // -1, -1 if all are active
    std::pair<bool, Condition> terminal;
};

