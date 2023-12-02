#pragma once

#include "sdl.h"

namespace engine::window {
  class Sprite {
  public:
    explicit Sprite(SDL_Texture*);

    ~Sprite();

    // - Variables -----------------------
    SDL_Texture* const &texture;
    const SDL_Rect &rect;
    const int &x, &y, &w, &h;


  private:
    // - Variables -----------------------
    SDL_Texture* m_texture;
    SDL_Rect m_rect{0, 0, 0, 0};


  };
}
