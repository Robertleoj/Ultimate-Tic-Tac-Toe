#pragma once

#include <game_ui.h>
#include <ui_static.h>
#include <SDL2/SDL.h>
#include <button.h>

enum MenuScreens{
    MAIN_MENU,
    CHOOSE_DIFF
};

enum MainMenuButtons{
    MM_TWO_PLAYER,
    MM_PLAY_AI,
    MM_EXIT
};

enum ChooseDiffButtons{
    CD_EASY,
    CD_MEDIUM,
    CD_HARD,
    CD_EXTREME,
    CD_BACK
};

class MainUI{
public:
    MainUI();
    ~MainUI();
    void run();

private:
    SDL_Window * window = nullptr;
    SDL_Renderer *renderer = nullptr;
    GameUI * gameui = nullptr;

    MenuScreens curr_menu;

    // SDL_Texture * game_name;
    // int game_name_height;

    int mousex = 0;
    int mousey = 0;

    // void play_ai(Difficulties d);

    bool init_SDL();
    bool init_menu_buttons();
    // bool init_game_name();
    void render_game_name();

    std::vector<Button *> main_menu_buttons;
    std::unordered_map<Button *, MainMenuButtons> main_menu_button_map;

    std::vector<Button *> diff_buttons;
    std::unordered_map<Button *, ChooseDiffButtons> choose_diff_button_map;

    // returns the button clicked
    // otherwise nullptr
    Button * get_clicked(
        std::vector<Button *> *buttons
    );

    // Render all the buttons in the vector
    void render_buttons(
        std::vector<Button *> *buttons
    );

    bool handle_main_menu_click();

    void render();

    bool handle_choose_diff_click();

    std::vector<Button *> get_menu_buttons(
        std::vector<std::string> names,
        int button_height,
        int button_width
    );

};
