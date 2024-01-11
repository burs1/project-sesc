#pragma once

#include <SDL.h>

namespace eng::sdl {

class Sprite {
public:
  explicit Sprite(SDL_Texture*);

  ~Sprite();

  // Vars
  SDL_Texture* const &texture;
  const SDL_Rect &rect;
  const int &x, &y, &w, &h;

private:
  // Vars
  SDL_Texture* texture_;
  SDL_Rect rect_{0, 0, 0, 0};

};

}
