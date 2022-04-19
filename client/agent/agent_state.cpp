#include <iostream>
#include <string>
#include <vector>
#include <exception>
#include <time.h>
#include <agent_state.h>
#include <static.h>




AgentState::AgentState(){

    num_board_wins = 0;
    turn = CROSS_TURN;
    board = new Condition ***[3];
    board_wins = new Condition *[3];
    all_moves = new Move ****[3];

    num_board_moves = new int *[3];

    for(int i = 0; i < 3; i++){
        board[i] = new Condition **[3];
        board_wins[i] = new Condition [3];
        all_moves[i] = new Move ***[3];
        num_board_moves[i] = new int[3];

        for(int j = 0; j < 3; j++){
            num_board_moves[i][j] = 0;
            board[i][j] = new Condition *[3];
            all_moves[i][j] = new Move **[3];
            board_wins[i][j] = EMPTY;

            for(int k = 0; k < 3; k++){
                board[i][j][k] = new Condition [3];
                all_moves[i][j][k] = new Move *[3];
                for(int l = 0; l < 3; l++){
                    board[i][j][k][l] = EMPTY;
                    all_moves[i][j][k][l] = new Move(i, j, k, l);
                }
            }
        }
    }
    
    this->num_moves = 0;
    this->move_sequence = new Move *[81];
}


AgentState::~AgentState(){
    // to delete:
    // board
    // board_wins
    // all_moves
    // num_board_moves
    // move_sequence

    for(int i = 0; i < 3; i++){
        for(int j = 0; j < 3; j++){
            for(int k = 0; k < 3; k++){
                for(int l = 0; l < 3; l++){
                    delete all_moves[i][j][k][l];
                }
                delete [] all_moves[i][j][k];
                delete [] board[i][j][k];
            }
            delete [] board[i][j];
            delete[] all_moves[i][j];
        }
        delete [] board_wins[i];
        delete[] num_board_moves[i];
        delete [] board[i];
        delete [] all_moves[i];
    }
    delete [] move_sequence;
    delete [] board;
    delete [] board_wins;
    delete [] all_moves;
    delete [] num_board_moves;
}



void AgentState::set_board(Move *m, Condition c){
    board[m->x1][m->y1][m->x2][m->y2] = c;
}


void AgentState::board_is_terminal(Condition **b, bool big_board, bool *is_term, Condition *cond, int sbx, int sby){

    if(big_board && num_board_wins < 3){
        *is_term = false;
        return;
    }

    if(!big_board && num_board_moves[sbx][sby] < 3){
        *is_term = false;
        return;
    }

    for(int i = 0; i < 3; i++){
        if(b[i][0] == b[i][1] && b[i][1] == b[i][2] && b[i][0] != EMPTY && b[i][0] != DRAW){
            // return the right heuristic
            *is_term = true;
            *cond = b[i][0];
            return;
        }

        if(b[0][i] == b[1][i] && b[1][i] == b[2][i] && b[0][i] != EMPTY && b[0][i] != DRAW){
            // return the right heuristic
            *is_term = true;
            *cond = b[0][i];
            return;
        }

    }
    if(b[0][0] == b[1][1] && b[1][1] == b[2][2] && b[0][0] != EMPTY && b[0][0] != DRAW){
        // return the right heuristic
        *is_term = true;
        *cond = b[0][0];
        return;
    }


    if(b[0][2] == b[1][1] && b[1][1] == b[2][0] && b[0][2] != EMPTY && b[0][2] != DRAW){
        // return the right heuristic
        *is_term = true;
        *cond = b[0][2];
        return;
    }


    if(big_board){
        if(num_board_wins < 9){
            *is_term = false;
            *cond = EMPTY;
            return;
        } 
    }else{
        if(num_board_moves[sbx][sby] < 9){
            *is_term = false;
            *cond = EMPTY;
            return;
        }

    }

    int num_squares_played = 0;
    int circle_wins = 0;
    int cross_wins = 0;
    for(int i = 0; i < 3; i++){
        for(int j = 0; j < 3; j++){
            num_squares_played += (b[i][j] != EMPTY);
            if(b[i][j] == CIRCLE){
                circle_wins++;
            }
            if(b[i][j] == CROSS){
                cross_wins++;
            }
        }
    }
    if(num_squares_played == 9){
        if(big_board){
            if(cross_wins > circle_wins){
                *is_term = true;
                *cond = CROSS;
                return;
            }
            if(cross_wins < circle_wins){
                *is_term = true;
                *cond = CIRCLE;
                return;
            }
        }
        *is_term = true; *cond = DRAW;
        return;
    }
    *is_term = false;
    *cond = EMPTY;
    return;
}


Condition AgentState::player_cond(){
    if(turn == CROSS_TURN){
        return CROSS;
    }
    return CIRCLE;
}

void AgentState::switch_turns(){
    if(turn == CROSS_TURN){
        turn = CIRCLE_TURN;
    } else{
        turn = CROSS_TURN;
    }
}

void AgentState::move(Move *m){
    
    set_board(m, player_cond());

    this->move_sequence[num_moves] = m;
    this->num_moves++;

    switch_turns();
    bool is_term; Condition cond;
    num_board_moves[m->x1][m->y1] ++;
    board_is_terminal(board[m->x1][m->y1], false, &is_term, &cond, m->x1, m->y1);

    if(is_term){
        board_wins[m->x1][m->y1] = cond;
        num_board_wins++;
    }
}



void AgentState::is_terminal(bool *is_term, Condition * cond){

    // First check if the board is won by either player
    if(num_board_wins < 3){
        *is_term = false;
        return;
    }

    bool bbterm;
    Condition bbcond;
    board_is_terminal(board_wins, true, &bbterm, &bbcond);
    if(bbterm){
        *is_term = true;
        *cond = bbcond;
        return;
    }

    *is_term = false;
    *cond = EMPTY;
    return;
}

void AgentState::undo_move(){
    // Get last move
    if(num_moves == 0){
        throw std::runtime_error("No moves have been played!");
    }
    Move * last_move = move_sequence[num_moves - 1];

    // change board_wins
    num_board_wins -= board_wins[last_move->x1][last_move->y1] != EMPTY ? 1 : 0;
    board_wins[last_move->x1][last_move->y1] = EMPTY;

    // change board
    set_board(last_move, EMPTY);
    num_board_moves[last_move->x1][last_move->y1]--;

    // change turn
    switch_turns();

    // change move_sequence
    num_moves--;
}

int AgentState::get_available_moves(Move ** move_buffer){

    // Check if we can play in the square played in
    int cnt = 0;
    if(num_moves > 0){
        Move *last_move = move_sequence[num_moves - 1];

        if(board_wins[last_move->x2][last_move->y2] == EMPTY){
            // We must play in that board
            for(int i = 0; i < 3; i ++ ){
                for(int j = 0; j < 3; j++){
                    if(board[last_move->x2][last_move->y2][i][j] == EMPTY){
                        move_buffer[cnt] = all_moves[last_move->x2][last_move->y2][i][j];
                        cnt++;
                    }
                }
            }

            return cnt;
        }
    } 

    // We can play in any board
    for(int i = 0; i < 3; i++){
        for(int j = 0; j < 3; j++){
            if(board_wins[i][j] == EMPTY){
                for(int k = 0; k < 3; k++){
                    for(int l = 0; l < 3; l++){
                        if(board[i][j][k][l] == EMPTY){
                            move_buffer[cnt] = all_moves[i][j][k][l];
                            cnt++;
                        }
                    }
                }
            }
        }
    }
    return cnt;

}