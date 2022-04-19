#pragma once

#include <SDL2/SDL.h>
#include <SDL2/SDL_ttf.h>
#include <unordered_map>
#include <ui_static.h>

class Assets{
public:

    Assets(SDL_Renderer * renderer);

    ~Assets();

    SDL_Texture * get_texture(Shapes s);

    void add_texture(std::string path, Shapes shape);


private:

    std::unordered_map<Shapes, SDL_Texture *> textures;

    SDL_Renderer *renderer = nullptr;
};

