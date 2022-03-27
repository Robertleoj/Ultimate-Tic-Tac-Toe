#pragma once

#include <agent_state.h>
#include <mc_tree.h>
#include <static.h>
#include <chrono>

class Agent {
public:
    Agent(int play_clock);

    ~Agent();

    void update(board_idx move);
    void update(Move *move);
    board_idx get_move();

private:

    std::chrono::steady_clock::time_point timer;
    Move ** move_buffer = nullptr;

    int play_clock;
    
    MCTree * tree = nullptr;
    AgentState * state = nullptr;

    Move * board_idx_to_move(board_idx b);

    board_idx move_to_board_idx(Move m);

    void start_timer();

    auto time_elapsed();

    bool time_out();

    void check_time();

    MCNode * selection();

    Condition simulation(MCNode * node);

    void backpropagation(MCNode *node, Condition sim_res);

    Move * get_current_best_move();

    double UCT(MCNode *node, MCNode * childnode);

};

