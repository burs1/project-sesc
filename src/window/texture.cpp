#include "window/texture.h"

namespace eng::window {
  
Texture::Texture(SDL_Texture* texture) : texture_(texture) {
  SDL_QueryTexture(texture_, NULL, NULL, &rect_.w, &rect_.h);
}


Texture::Texture(SDL_Texture* texture, SDL_Rect rect)
  : texture_(texture), rect_(rect) {}


Texture::~Texture() {
  SDL_DestroyTexture(texture_);
}


// ~ Getters
auto Texture::GetSize(int* width, int* height) const -> void {
  *width = rect_.w;
  *height = rect_.h;
}


auto Texture::GetTexture() const -> SDL_Texture* {
  return texture_;
}

  
}
