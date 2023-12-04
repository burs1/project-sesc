#pragma once

#include <sdl.h>

namespace engine::window {
  class Sprite {
  public:
    explicit Sprite(SDL_Texture*);

    ~Sprite();

    // vars
    SDL_Texture* const &texture;
    const SDL_Rect &rect;
    const int &x, &y, &w, &h;

  private:
    // vars
    SDL_Texture* m_texture;
    SDL_Rect m_rect{0, 0, 0, 0};

  };
}
