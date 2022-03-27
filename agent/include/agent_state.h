#pragma once
#include <string>
#include <vector>
#include <static.h>
#include <move.h>


class AgentState {
public:
    Turn turn;

    AgentState();

    ~AgentState();

    void move(Move *idx);

    void undo_move();

    void is_terminal(bool *is_term, Condition *cond);

    int get_available_moves(Move ** move_buffer);

    Move *****all_moves;


private:

    int num_moves;
    Move ** move_sequence;

    Condition player_cond();

    void board_is_terminal(Condition **b, bool big_board, bool *is_term, Condition *cond, int sbx = 0, int sby = 0);

    Condition ****board;
    Condition **board_wins;
    int num_board_wins;
    int **num_board_moves;

    void switch_turns();
    void set_board(Move *idx, Condition c);
};

