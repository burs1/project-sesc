#include "sprite.h"

namespace engine::window {
  Sprite::Sprite(SDL_Texture* texture)
  : texture(m_texture), x(m_rect.x), y(m_rect.y), w(m_rect.w), h(m_rect.h), rect(m_rect) {
    m_texture = texture;

    SDL_QueryTexture(m_texture, NULL, NULL, &m_rect.w, &m_rect.h);
  }

  Sprite::~Sprite() {
    SDL_DestroyTexture(m_texture);
  }
}
