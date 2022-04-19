#pragma once

#include <SDL2/SDL.h>
#include <assets.h>
#include <static.h>

class Cell {
public:

    Cell(SDL_Renderer *renderer, Assets *media, int x1, int y1, int x2, int y2, board_idx idx);

    ~Cell();

    void render(StateData * state_data, bool board_active, int mousex, int mousey);

    bool in_square(int x, int y);

private:

    int x_start, y_start, x_end, y_end;
    board_idx idx;
    Assets * media = nullptr;
    SDL_Renderer * renderer = nullptr;
    SDL_Rect cell_rect;
    SDL_Color highlight_color = {0, 255, 26, 100};

};
