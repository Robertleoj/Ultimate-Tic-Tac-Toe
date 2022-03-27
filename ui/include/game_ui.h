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


class GameUI{
public:

    GameUI(SDL_Renderer * renderer);
    bool run(bool play_ai, Difficulties d=EASY);

    ~GameUI();

private:

    SDL_Renderer *renderer = nullptr;
    SmallBoard *** boards = nullptr;

    Agent * agent = nullptr;
    

    Game * game = nullptr;
    Text * text_class = nullptr;
    StateData state_data;

    SDL_Texture * won_message = nullptr;

    Button * play_again_button = nullptr;
    Button * restart_game_button = nullptr;
    Button * back_to_main_menu_button = nullptr;

    Assets * media = nullptr;

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

    bool playing_ai;
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
    void init_ai();

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

    void init_game();
    void render();

    void reset_won_msg();

    void close();
};
