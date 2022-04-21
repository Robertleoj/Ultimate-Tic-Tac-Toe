#pragma once
#include <SDL2/SDL.h>
#include <SDL2/SDL_ttf.h>
#include <string>
#include <small_board.h>
#include <assets.h>
#include <static.h>
#include <game.h>
#include <text.h>
#include <button.h>
#include <agent.h>
#include <memory>
#include <connection.h>
#include <mutex>
#include <thread>


class GameUI{
public:

    GameUI(SDL_Renderer * renderer);
    bool run(PlayMode play_mode, Difficulties d=EASY);

    ~GameUI();

private:

    // SDL_objects cannot be stored with std::unique_ptr, as their deconstructor
    // cannot be called. 
    SDL_Renderer *renderer = nullptr;
    SDL_Texture * won_message = nullptr;

    SmallBoard *** boards = nullptr;

    std::unique_ptr<Agent> agent;

    std::unique_ptr<Connection> connection;

    std::unique_ptr<Game> game;
    std::unique_ptr<Text> text_class;
    StateData state_data;


    std::unique_ptr<Button> play_again_button;
    std::unique_ptr<Button> restart_game_button;
    std::unique_ptr<Button> back_to_main_menu_button;

    std::unique_ptr<Assets> media;

    int mousex = 0;
    int mousey = 0;

    int x_board_start;
    int x_board_end;
    int y_board_start;
    int y_board_end;
    int board_width;
    int line_incr;
    int board_incr;
    int thick_line_width = 4;

    // only when playing online
    Turn whoami;

    // opponent thread variables --
    std::mutex opponent_move_mutex;
    std::unique_ptr<std::thread> opponent_thread;
    bool thread_running; 

    // shared data -- use mutex to use
    bool opponent_made_move; 
    board_idx opponent_move;
    
    // ---------------------------

    // bool playing_ai;
    PlayMode play_mode;
    int ai_play_clock;

    SDL_Rect won_message_rect;
    LineStyle thick_line_style = {
        {255, 28, 149, 0xff}, thick_line_width
    };

    LineStyle highlight_line_style = {
        {50, 227, 50, 0xff}, thick_line_width
    };

    SDL_Texture *load_texture(std::string path);

    void make_won_msg();
    void make_won_msg_rect(int w, int h);
    void init_ai_class();

    void init_text_class();
    void restart_game();

    void init_buttons();
    void render_buttons();

    void render_won_msg();

    bool load_media();

    void init_board();
    void draw_board();
    bool handle_click();

    void render_small_boards();
    void init_game_class();
    void render();
    void init_match();

    void init_connection();
    void start_opponent_move();
    void check_opponent_move();
    void start_online_opponent_move();
    void start_ai_opponent_move();
    bool our_move();

    void reset_won_msg();
    void tasks();

    void close();
};
