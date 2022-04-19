#pragma once

#include<SDL2/SDL.h>
#include <ui_static.h>
#include <unistd.h>

void set_render_color(SDL_Renderer * renderer, SDL_Color c);

void draw_line(SDL_Renderer * renderer, int x1, int y1, int x2, int y2, LineStyle style);

void set_background_color(SDL_Renderer * renderer);

void frame_delay();

