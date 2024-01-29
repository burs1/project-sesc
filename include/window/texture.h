#pragma once

#include <SDL.h>

namespace eng::window {

class Texture {
public:
  // Constructor
  explicit Texture(SDL_Texture*);

  // Extended constructor
  Texture(SDL_Texture*, SDL_Rect);

  // Destructor
  ~Texture();

  // - Methods -
  // Assigns texture size in pixels to passed pointers.
  auto GetSize(int*, int*) const -> void;

  // Returns an SDL texture pointer.
  auto GetTexture()        const -> SDL_Texture*;

private:
  // Vars
  SDL_Texture* texture_;
  SDL_Rect rect_;

};

}
