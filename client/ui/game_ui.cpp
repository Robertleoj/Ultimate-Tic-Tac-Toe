
#include <game_ui.h>
#include <SDL2/SDL_image.h>
#include <iostream>
#include <stdio.h>
#include <ui_static.h>
#include <ui_utils.h>
#include <unistd.h>


GameUI::GameUI(SDL_Renderer * renderer){
    // agent = new Agent(1000);
    this->renderer = renderer;
    init_game_class();
    init_text_class();
    load_media();
    init_board();
    init_buttons();
}

GameUI::~GameUI(){

    // delete smallboards
    for(int i = 0; i < 3; i++){
        for(int j =0; j < 3; j++){
            delete boards[i][j];
        }
        delete[] boards[i];
    }

    delete[] boards;

    if (won_message != nullptr){
        SDL_DestroyTexture(won_message);
    }
}

void GameUI::reset_won_msg(){
    if(won_message != nullptr){
        SDL_DestroyTexture(won_message);
        this->won_message = nullptr;
    }
}

void GameUI::restart_game(){
    reset_won_msg();
    this->init_game_class();

    if(this->play_mode == PM_PLAY_AI){
        init_ai_class();
    }
}


void GameUI::init_ai_class(){
    this->agent = std::unique_ptr<Agent>(new Agent(this->ai_play_clock));
}

void GameUI::init_game_class(){
    // if(this->game != nullptr){
    //     delete game;
        
    // }
    this->game = std::unique_ptr<Game>(new Game());
    state_data = game->get_state_data();
}

void GameUI::init_connection(){
    this->connection = std::unique_ptr<Connection>(new Connection());
    this->whoami = connection->init_connection();

}

void GameUI::init_text_class(){
    this->text_class = std::unique_ptr<Text>(new Text(renderer));
}

SDL_Texture * GameUI::load_texture(std::string path){
    // The final texture
    SDL_Texture * new_texture = nullptr;

    SDL_Surface * loaded_surface = IMG_Load(path.c_str());

    if(loaded_surface == nullptr){
        std::cout << "Unable to load image at " << path << ". Error: " << IMG_GetError() << std::endl;
        return nullptr;
    }

    // create texture from surface pixels
    new_texture = SDL_CreateTextureFromSurface(renderer, loaded_surface);

    if(new_texture == nullptr){
        std::cout << "Could not get texture from surface. Error: " << SDL_GetError() << std::endl;
        return nullptr;
    }

    // get rid of loaded surface
    SDL_FreeSurface(loaded_surface);

    return new_texture;
}



bool GameUI::load_media(){
    this->media = std::unique_ptr<Assets>(new Assets(renderer));
    media->add_texture("cross.png", CROSS_SHAPE);
    media->add_texture("circle.png", CIRCLE_SHAPE);
    return true;
}

void GameUI::init_board(){
    x_board_start = screen_width / 9;
    x_board_end = (8 * screen_width ) / 9;

    board_width = (x_board_end - x_board_start);

    y_board_start = (screen_height - board_width) / 2;
    y_board_end = y_board_start + board_width;

    boards = new SmallBoard **[3];
    for(int i = 0; i < 3; i++){
        boards[i] = new SmallBoard *[3];
    }

    board_incr = board_width / 3;
    int x1, y1, x2, y2;
    for(int i = 0; i < 3; i++){ // x coord
        for(int j = 0; j < 3; j++){ // y coord
            x1 = (x_board_start + board_incr * i + thick_line_width);
            y1 = y_board_start + board_incr * j + thick_line_width;
            x2 = x1 + board_incr - thick_line_width*2;
            y2 = y1 + board_incr - thick_line_width*2;
            
            boards[i][j] = new SmallBoard(renderer,media.get(),x1, y1, x2, y2, std::make_pair(i, j));
        }
    }
}

void GameUI::draw_board(){
    

    int xstart, ystart, xend, yend;

    for(int i = 0; i < 4; i ++){
        // Draw vertical line
        xstart = x_board_start + i * board_incr;
        // xstart = x_board_start;
        xend = xstart;
        ystart = y_board_start;
        yend = y_board_end;
        draw_line(renderer, xstart, ystart, xend, yend, thick_line_style);

        // draw horizontal line
        xstart = x_board_start;
        xend = x_board_end;
        ystart = y_board_start + i * board_incr;
        yend = ystart;
        draw_line(renderer, xstart, ystart, xend, yend, thick_line_style);
    }

    // Draw active board

    if(!state_data.terminal.first){
        auto active_board = this->state_data.active_board;

        if(active_board.first == -1 && active_board.second == -1){
            // Draw around the whole board
            xstart = x_board_start;
            xend = x_board_start + 3 * board_incr;
            ystart = y_board_start;
            yend = y_board_start + 3 * board_incr;

        } else {
            // Draw around the active dude
            xstart = x_board_start + active_board.first * board_incr;
            xend = xstart + board_incr;
            ystart = y_board_start + active_board.second * board_incr;
            yend = ystart + board_incr;
        }

        // horizontal
        draw_line(renderer, xstart, ystart, xend, ystart, highlight_line_style);
        draw_line(renderer, xstart, yend, xend, yend, highlight_line_style);

        //vertical
        draw_line(renderer, xstart, ystart, xstart, yend, highlight_line_style);
        draw_line(renderer, xend, ystart, xend, yend, highlight_line_style);
    }
}


void GameUI::render_small_boards(){
    for(int i = 0; i < 3; i++){
        for(int j = 0; j < 3; j++){
            boards[i][j]->render(&this->state_data, mousex, mousey);
        }
    }
}

void GameUI::make_remote_move(){
    board_idx opponent_move = this->connection->get_move();
    this->game->move(opponent_move);
    this->state_data = game->get_state_data();
}

bool GameUI::handle_click(){

    
    if(
        !state_data.terminal.first 
        && !(this->play_mode == PM_ONLINE 
                && this->whoami != this->state_data.turn)
    ){

        for(int i = 0; i < 3; i++){
            for(int j = 0; j < 3; j++){

                if(this->boards[i][j]->in_board(mousex, mousey)){
                    std::pair<int, int> cell_idx = this->boards[i][j]->which_cell(mousex, mousey);
                    if(cell_idx.first != -1 && cell_idx.second != -1){
                        // See if this is an available move
                        board_idx move = {i, j, cell_idx.first, cell_idx.second};

                        for(auto m : this->state_data.allowed_moves){
                            if(m[0] == move[0] && m[1] == move[1] && m[2] == move[2] && m[3] == move[3]){
                                this->game->move(move);
                                this->state_data = game->get_state_data();

                                if(this->play_mode == PM_PLAY_AI && !this->state_data.terminal.first){
                                    this->render();
                                    this->agent->update(move);
                                    board_idx ai_move = this->agent->get_move();

                                    this->game->move(ai_move);
                                    this->state_data = game->get_state_data();
                                }

                                if(
                                    this->play_mode == PM_ONLINE 
                                    && !this->state_data.terminal.first 
                                ){
                                    this->connection->send_move(move);
                                    this->render();
                                    make_remote_move();
                                }

                                return false;
                            }
                        }
                    }
                }
            }
        }
    }

    if(this->state_data.terminal.first){
        // Check whether the player pressed play again
        if(this->play_again_button->on_button(mousex, mousey)){
            // restart game
            restart_game();
            return false;
        }
    } else {
        if(this->restart_game_button->on_button(mousex, mousey)){
            // restart game
            restart_game();
            return false;
        }
    }

    if(this->back_to_main_menu_button->on_button(mousex, mousey)){
        return true;
    }

    return false;
}

void GameUI::make_won_msg_rect(int w, int h){

    int xpos = screen_width / 2 - w / 2;

    this->won_message_rect = {
        xpos, 0, w, h
    };
}


void GameUI::make_won_msg() {
    Condition result = this->state_data.terminal.second;
    int text_w, text_h;
    int fontsize = 70;
    Fonts font = CAVIAR_BOLD;
    switch (result) {
        case CROSS: {
            this->won_message = text_class->get_text(
                font,
                "X's Win!", 
                ui_colors.at(CROSS_WON_COLOR), 
                fontsize, 
                &text_w,
                &text_h
            );
        } break;
        case CIRCLE: {
            this->won_message = text_class->get_text(
                 font, 
                 "O's Win!", 
                 ui_colors.at(CIRCLE_WON_COLOR),
                 fontsize,
                 &text_w,
                 &text_h
            );
        } break;
        case DRAW: {
            this->won_message = text_class->get_text(
                font,
                "Draw!", 
                ui_colors.at(CROSS_WON_COLOR), 
                fontsize,
                &text_w,
                &text_h
            );
        } break;    
    }
    make_won_msg_rect(text_w, text_h);
}

void GameUI::render_won_msg(){
    auto term = state_data.terminal;

    if(!term.first){
        // Game is still on
        return;
    }


    if(this->won_message == nullptr){
        make_won_msg();
    }


    if(SDL_RenderCopy(renderer, this->won_message, nullptr, &this->won_message_rect) == -1){
        printf("Could not render font at. Error: %s\n", SDL_GetError());
    }
}

void GameUI::init_buttons(){
    int space_below = screen_height - y_board_end;

    int height = 3*(screen_height - y_board_end)  / 4;

    int width = 300;
    int y_start = y_board_end + (space_below - height) / 2;
   
    int y_end = y_start + height;

    int space_between = 40;

    int first_button_x_start = screen_width/2 - (2 * width + space_between) / 2;

    SDL_Rect button_rect = {
        first_button_x_start,
        y_start,
        width,
        height
    };

    // make play again button

    this->play_again_button = std::unique_ptr<Button>(new Button(
        renderer, 
        button_rect,
        "Play again?"
    ));

    // make restart button

    this->restart_game_button = std::unique_ptr<Button>(new Button(
        renderer, 
        button_rect,
        "Restart Game"
    ));

    // make back button

    button_rect.x += width + space_between;
    
    this->back_to_main_menu_button = std::unique_ptr<Button>(new Button(
        renderer,
        button_rect,
        "Back"
    ));

}

void GameUI::render_buttons(){
    //render back button

    this->back_to_main_menu_button->render(mousex, mousey);
    if(this->state_data.terminal.first){
        //render play again buttom 
        this->play_again_button->render(mousex, mousey);
    }else{
        //render replay button 
        this->restart_game_button->render(mousex, mousey);
    }
}

void GameUI::init_match(){
    init_game_class();
    if(this->play_mode == PM_PLAY_AI){
        init_ai_class();
    } else if(this->play_mode == PM_ONLINE){
        init_connection();
    }
}

void GameUI::render(){

    set_background_color(renderer);
    SDL_RenderClear(renderer);
    draw_board();
    render_small_boards();
    render_won_msg();
    render_buttons();
    SDL_RenderPresent(renderer);
}

bool GameUI::run(PlayMode play_mode, Difficulties diff){
    this->play_mode = play_mode;

    this->ai_play_clock = difficulty_playclock_map.at(diff);

    init_match();
    reset_won_msg();
    render();

    // quick fix - should be fixed later with multithreading
    if(this->play_mode == PM_ONLINE
        && this->state_data.turn != this->whoami
    ){
        make_remote_move();
    }

    bool quit = false;
    bool close_program = false;

    SDL_Event e;

    while(!quit){
        while(SDL_PollEvent(&e) != 0){
            if(e.type == SDL_QUIT){
                quit = true;
                close_program = true;
            }
            else if(e.type == SDL_MOUSEMOTION){
                SDL_GetMouseState(&mousex, &mousey);
            } else if(e.type == SDL_MOUSEBUTTONDOWN){
                SDL_GetMouseState(&mousex, &mousey);
                quit = handle_click();
            }
        }
        render();
        
        frame_delay();
    }
    return close_program;
}


