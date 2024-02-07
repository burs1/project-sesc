#pragma once

#include <SDL.h>

namespace eng::window {

class Texture {
 public:
  explicit Texture(SDL_Texture*);
  Texture(SDL_Texture*, SDL_Rect);
  ~Texture();

  // Assigns texture size in pixels to passed pointers.
  auto GetSize(int*, int*) const -> void;

  // Returns a pointer to an SDL_Texture object.
  auto GetTexture()        const -> SDL_Texture*;

 private:
  SDL_Texture* texture_;
  SDL_Rect rect_;
};

}

