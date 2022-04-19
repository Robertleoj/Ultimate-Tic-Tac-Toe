#include <ui_utils.h>
#include<SDL2/SDL2_gfxPrimitives.h>
#include <ui_static.h>


void set_render_color(SDL_Renderer * renderer, SDL_Color c){
    SDL_SetRenderDrawBlendMode(renderer, SDL_BLENDMODE_BLEND);
    SDL_SetRenderDrawColor(
        renderer,c.r,c.g,c.b,c.a
    );
}

void draw_line(SDL_Renderer * renderer, int x1, int y1, int x2, int y2, LineStyle style){
    SDL_SetRenderDrawBlendMode(renderer, SDL_BLENDMODE_BLEND);
    thickLineRGBA(renderer, x1, y1, x2, y2, style.thickness, style.c.r, style.c.g, style.c.b, style.c.a);
}

void set_background_color(SDL_Renderer * renderer){
    SDL_Color background_color = ui_colors.at(BACKGROUND_COLOR);
    SDL_SetRenderDrawColor(renderer, background_color.r, background_color.g, background_color.b, background_color.a);
}

void frame_delay(){
    usleep(FRAME_DELAY);
}
