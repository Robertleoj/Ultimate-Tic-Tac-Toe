#include <main_ui.h>
#include <text.h>


MainUI::MainUI(){
    init_SDL();
    // game_name_height = 100;
    // init_game_name();
    init_menu_buttons();

    this->gameui = new GameUI(renderer);
    this->curr_menu = MAIN_MENU;
}

MainUI::~MainUI(){
    if (gameui != nullptr) {
        delete this->gameui;
    }

    for(auto &button : main_menu_buttons){
        delete button;
    }
    
    
    for (auto &button : diff_buttons) {
        delete button;
    }   

    SDL_DestroyWindow(window);
    SDL_DestroyRenderer(renderer);
    SDL_Quit();
}


std::vector<Button *> MainUI::get_menu_buttons(
    std::vector<std::string> names,
    int button_width,
    int button_height
){

    int num_buttons = names.size();
    int button_interval = screen_height / (num_buttons + 1);

    SDL_Rect button_rect = {
        screen_width / 2 - button_width / 2,
        button_interval - button_height / 2,
        button_width,
        button_height
    };

    std::vector<Button *> button_vector;

    for(auto &s: names){
        Button * new_button = new Button(
            renderer,
            button_rect,
            s,
            60
        );
        button_vector.push_back(new_button);
        button_rect.y += button_interval;
    }

    return button_vector;
}


bool MainUI::init_menu_buttons(){
    int button_width = 400;
    int button_height = 130;

    // make main menu buttons
    this->main_menu_buttons = get_menu_buttons(
        {"Two player", "Play AI", "Exit"},
        button_width,
        button_height
    );
    // make map
    this->main_menu_button_map = std::unordered_map<Button *, MainMenuButtons>{
        {main_menu_buttons[0], MM_TWO_PLAYER},
        {main_menu_buttons[1], MM_PLAY_AI},
        {main_menu_buttons[2], MM_EXIT}
    };

    // make choose diff buttons
    this->diff_buttons = get_menu_buttons(
        {"Easy", "Medium", "Hard", "Extreme", "Back"},
        button_width,
        button_height
    );

    this->choose_diff_button_map = std::unordered_map<Button *, ChooseDiffButtons>{
        {diff_buttons[0], CD_EASY},
        {diff_buttons[1], CD_MEDIUM},
        {diff_buttons[2], CD_HARD},
        {diff_buttons[3], CD_EXTREME},
        {diff_buttons[4], CD_BACK}
    };
    return true;
}




bool MainUI::init_SDL(){

    if(SDL_Init(SDL_INIT_VIDEO) < 0){
        std::cout << "Could not initialize window! Err" << SDL_GetError() << std::endl;
        return false;
    }

    window = SDL_CreateWindow(
        "Ultimate Tic-Tac-Toe",
        SDL_WINDOWPOS_UNDEFINED,
        SDL_WINDOWPOS_UNDEFINED,
        screen_width,
        screen_height,
        SDL_WINDOW_SHOWN
    );

    if(window == nullptr){
        std::cout << "Could not get window! Err" << SDL_GetError() << std::endl;
    }

    // create renderer for window
    renderer = SDL_CreateRenderer(window, -1, SDL_RENDERER_ACCELERATED);

    if(renderer == nullptr){
        std::cout << "Could not create renderer! Error: " << SDL_GetError() << std::endl;
        return false;
    }

    set_background_color(renderer);

    return true;
}

bool MainUI::handle_main_menu_click(){
    Button * button_clicked = get_clicked(
        &this->main_menu_buttons
    );

    if(button_clicked == nullptr){
        return false;
    }

    MainMenuButtons clicked = main_menu_button_map[button_clicked];

    switch(clicked){
        case MM_TWO_PLAYER:
            return this->gameui->run(false);
        case MM_PLAY_AI:
            this->curr_menu = CHOOSE_DIFF;
            return false;
        case MM_EXIT:
            return true;
        default:
            throw std::runtime_error("Invalid state");
    }
}

void MainUI::render_buttons(
    std::vector<Button *> *buttons
){
    for(auto &b : *buttons){
        b->render(mousex, mousey);
    }
}

Button * MainUI::get_clicked(
    std::vector<Button *> *buttons
){
    for(auto &b: *buttons){
        if(b->on_button(mousex, mousey)){
            return b;
        }
    }
    return nullptr;
}

bool MainUI::handle_choose_diff_click(){
    Button * button_clicked = get_clicked(
        &this->diff_buttons
    );

    if(button_clicked == nullptr){
        return false;
    }

    ChooseDiffButtons clicked = this->choose_diff_button_map.at(button_clicked);

    this->curr_menu = MAIN_MENU;

    Difficulties diff;
    switch(clicked){
        case CD_BACK:
            return false;
        case CD_EASY:
            diff = EASY; break;
        case CD_MEDIUM:
            diff = MEDIUM; break;
        case CD_HARD:
            diff = HARD; break;
        case CD_EXTREME:   
            diff = EXTREME; break;
    }
    return this->gameui->run(true, diff);
}

void MainUI::render(){
    set_background_color(renderer);
    SDL_RenderClear(renderer);
    // render_game_name();
    switch(curr_menu){
        case MAIN_MENU:
            this->render_buttons(&this->main_menu_buttons);
            break;
        case CHOOSE_DIFF:
            this->render_buttons(&this->diff_buttons);
            break;
    }
    SDL_RenderPresent(renderer);
}

void MainUI::run(){
    bool quit = false;

    SDL_Event e;

    while(!quit){
        while(SDL_PollEvent(&e) != 0){
            if(e.type == SDL_QUIT){
                quit = true;
            }
            if(e.type == SDL_MOUSEMOTION){
                SDL_GetMouseState(&mousex, &mousey);
            }
            else if(e.type == SDL_MOUSEBUTTONDOWN){
                switch(this->curr_menu){
                    case MAIN_MENU:
                        quit = handle_main_menu_click();
                        break;
                    case CHOOSE_DIFF:
                        quit = handle_choose_diff_click();
                        break;
                }
            }
        }
        this->render();
        frame_delay();
    }
}