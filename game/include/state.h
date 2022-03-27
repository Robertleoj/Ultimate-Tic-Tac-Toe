#pragma once
#include <iostream>
#include <string>
#include <vector>
#include <algorithm>
#include <exception>
#include <time.h>
#include <static.h>


class State {
public:
    State();
    ~State();

    void move(board_idx idx);

    std::pair<bool, Condition> is_terminal();

    std::pair<std::vector<board_idx>, std::pair<int, int>> get_available_moves();

    Condition ****board;
    Condition **board_wins;
    Turn turn;

private:
    std::vector<board_idx> move_sequence;

    void switch_turns();
    Condition player_cond();

    std::pair<bool, Condition> board_is_terminal(Condition **b, bool big_board);

    void set_board(board_idx &idx, Condition c);

};
