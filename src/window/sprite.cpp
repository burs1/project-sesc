#include "window/sprite.h"

namespace eng::sdl {
  
Sprite::Sprite(SDL_Texture* texture)
: texture(texture_), x(rect_.x), y(rect_.y),
  w(rect_.w), h(rect_.h), rect(rect_) {
  texture_ = texture;

  SDL_QueryTexture(texture_, NULL, NULL, &rect_.w, &rect_.h);
}


Sprite::~Sprite() {
  SDL_DestroyTexture(texture_);
}

  
}
