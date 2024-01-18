#pragma once

#include <SDL.h>
#include <SDL_render.h>

namespace eng::window {

class Texture {
public:
  explicit Texture(SDL_Texture*);

  Texture(SDL_Texture*, SDL_Rect);

  ~Texture();

  // ~ Getters
  auto GetSize(int*, int*) const -> void;

  auto GetTexture() const -> SDL_Texture*;

private:
  // Vars
  SDL_Texture* texture_;
  SDL_Rect rect_;

};

}
