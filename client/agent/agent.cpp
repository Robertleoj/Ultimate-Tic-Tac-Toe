#include <agent.h>
#include <agent_static.h>
#include <math.h>
#include <random>


Agent::Agent(int play_clock){
    this->state = new AgentState();
    this->tree = new MCTree();
    this->play_clock = play_clock;
    this->move_buffer = new Move*[81];
}

Agent::~Agent(){
    if (this->tree != nullptr) {
        delete this->tree;
    }

    delete[] this->move_buffer;

    if (this->state != nullptr) {
       delete this->state;
    }
}

void Agent::update(Move *move){
    state->move(move);
    tree->move(move);
}

void Agent::update(board_idx move){
    // Update state
    Move* m = this->state->all_moves[move[0]][move[1]][move[2]][move[3]];
    this->update(m);
}

double Agent::UCT(MCNode * node, MCNode * childnode){
    /*
    UCT(node, move) = 
        (w / n) + c * sqrt(ln(N) / n)

    w = wins for child node after move
    n = number of simulations for child node
    N = simulations for current node
    c = sqrt(2)
    */
    if(childnode == nullptr){
        throw std::runtime_error("No child node");
    }

    double w = childnode->wins;
    double n = childnode->plays;
    double N = node->plays;
    double c = sqrt(2);
    
    return (w / n) + c * sqrt(log(N) / n);

}

MCNode * Agent::selection(){

    MCNode * current_node = tree->root;
    double max_uct;
    double uct;
    MCNode * next_node = nullptr;
    Move *best_move;

    std::map<Move *, MCNode *> *child_map;
    MCNode * child_node = nullptr;

    while(true){
        // find best node
        if(current_node->is_terminal){
            return current_node;
        }

        max_uct = -100000;
        int num_moves = state->get_available_moves(move_buffer);

        child_map = &current_node->children;

        for(int i = 0; i < num_moves; i++){
            Move * m = move_buffer[i];
            if(child_map->find(m) == child_map->end()){
                // Make new node 
                MCNode * new_node = new MCNode(current_node);
                current_node->children[m] = new_node;
                
                // update state
                state->move(m);

                // return new node
                return new_node;
    
            } else {
                
                // Get uct value
                child_node = child_map->at(m);
                uct = UCT(current_node, child_node);

                // Update 
                if(uct > max_uct){
                    max_uct = uct;
                    next_node = child_node;
                    best_move = m;
                }
            }
        }
        state->move(best_move);
        current_node = next_node;
        next_node = nullptr;
        child_node = nullptr;
    }
}


Condition Agent::simulation(MCNode *selected_node) {
    int cnt = 0;
    int num_moves;
    int rand_idx;
    bool stateterm;
    Condition wincond;

    while (true) {
        
        state->is_terminal(&stateterm, &wincond);
        
        if(stateterm){
            // if counter == 0, update selected node to be terminal
            if(cnt == 0) {
                selected_node->is_terminal = true;
            }
            break;
        }

        // Select random move
        num_moves = state->get_available_moves(move_buffer);
        rand_idx = rand() % num_moves;
        state->move(move_buffer[rand_idx]);
        cnt ++;
    }

    for(int i = 0; i < cnt; i++){
        state->undo_move();
    }

    return wincond;
};

void Agent::backpropagation(MCNode * node, Condition sim_res){
    while(true){
        node->plays++;
        switch(sim_res){
            case DRAW:
                node->wins += 0.5;
                break;

            case CIRCLE:
                node->wins += (state->turn == CIRCLE_TURN ? 0 : 1);
                break;
            
            case CROSS:
                node->wins += (state->turn == CROSS_TURN ? 0 : 1);
                break;
        }


        if (node->parent == nullptr) {
            break;
        } else {
            state->undo_move();
            node = node->parent;
        }
    }
}

Move * Agent::get_current_best_move(){
    double highest = -10000;
    Move *best_move;
    auto child_map = tree->root->children;
    double score;
    
    for(auto &p : child_map){
        score = p.second->wins / p.second->plays;
        if(score > highest){
            highest = score;
            best_move = p.first;
        }
    }

    return best_move;
}


board_idx Agent::get_move(){
    start_timer();
    srand(time(NULL));

    int i = 0;
    while(!time_out()){
        i++;

        MCNode * selected_node = selection();

        Condition sim_res = simulation(selected_node);

        backpropagation(selected_node, sim_res);
    }

    // Get best move
    printf("Performed %d iterations\n", i);
    Move * ret_move = get_current_best_move();
    update(ret_move);
    return move_to_board_idx(*ret_move);

}

Move * Agent::board_idx_to_move(board_idx b){
    return this->state->all_moves[b[0]][b[1]][b[2]][b[3]];
}


board_idx Agent::move_to_board_idx(Move m){
    return board_idx {m.x1, m.y1, m.x2, m.y2};
}


// time elapsed in ms since we started the timer
auto Agent::time_elapsed(){
    return since(timer).count();
}

// Check if we have timed out
bool Agent::time_out(){
    return time_elapsed() > play_clock;
}

void Agent::start_timer(){
    timer = std::chrono::steady_clock::now();
}

