#include "window/drawer.h"
#include <stdexcept>

namespace eng::sdl {

Drawer* Drawer::kInstance = nullptr;

// Static methods
auto Drawer::Init(SDL_Window* sdl_window) -> void {
  if (not kInstance) {
    kInstance = new Drawer(sdl_window);
    return;
  }
  throw std::runtime_error("Drawer is already online.");
}

auto Drawer::GetInstance() -> Drawer* {
  if (kInstance) {
    return kInstance;
  }
  throw std::runtime_error("Drawer is offline.");
}

auto Drawer::Quit() -> void {
  if (kInstance) {
    delete kInstance;
    return;
  }
  throw std::runtime_error("Drawer is already offline.");
}

Drawer::Drawer(SDL_Window* sdl_window)
: resw(resw_), resh(resh_) {
  // Initialize SDL_ttf
  if (TTF_Init() < 0) {
    throw std::runtime_error(TTF_GetError());
  }

  // Init IMG
  if (IMG_Init(IMG_INIT_PNG) < 0) {
    throw std::runtime_error(IMG_GetError());
  }

  // Create renderer
  sdl_renderer_ = SDL_CreateRenderer(
                    sdl_window,
                    -1,
                    SDL_RENDERER_ACCELERATED);

  // Check if renderer was successfully created
  if (sdl_renderer_ == NULL) {
    throw std::runtime_error(SDL_GetError());
  }

  SDL_GetWindowSize(sdl_window, &resw_, &resh_);
}

Drawer::~Drawer() {
  // Unload all loaded fonts
  for (auto [name, font] : fonts_) {
    TTF_CloseFont(font);
  }

  // Unload all loaded sprites
  for (auto [name, sprite] : sprites_) {
    delete sprite;
  }

  SDL_DestroyRenderer(sdl_renderer_);
  TTF_Quit();
  IMG_Quit();
}


// Methods
// ~ Main
auto Drawer::Present() -> void {
  SDL_RenderPresent(sdl_renderer_);

  auto& [cr, cg, cb, ca] = clear_color_;
  SDL_SetRenderDrawColor(sdl_renderer_, cr, cg, cb, ca);

  SDL_RenderClear(sdl_renderer_);

  auto& [dr, dg, db, da] = draw_color_;
  SDL_SetRenderDrawColor(sdl_renderer_, dr, dg, db, da);
}


// ~ Resources
auto Drawer::LoadSprite(const char *path, const char *name) -> void {
  if (sprites_.contains(name)) {
    throw std::runtime_error("Sprite with name \"" + std::string(name) + "\" already exists");
  }

  SDL_Texture *texture = IMG_LoadTexture(sdl_renderer_, path);
  
  // Check if texture was successfuly loaded
  if (texture == NULL) {
    throw std::runtime_error(SDL_GetError());
  }

  sprites_[name] = new Sprite(texture);
}


auto Drawer::UnloadSprite(const char* name) -> void {
  delete sprites_[name];
  sprites_.erase(name);
}


auto Drawer::LoadFont(const char* file, const char* name, int size) -> void{
  if (fonts_.contains(name)) {
    throw std::runtime_error("Font with name \"" + std::string(name) + "\" already has been loaded");
  }

  fonts_[name] = TTF_OpenFont(file, size);

  // Check if font was successfully loaded
  if (fonts_[name] == NULL) {
    throw std::runtime_error(TTF_GetError());
  }
}

auto Drawer::UnloadFont(const char* name) -> void {
  TTF_CloseFont(fonts_[name]);
  fonts_.erase(name);
}


// ~ Draw
auto Drawer::SetRenderLogicalSize(int w, int h) -> void {
  SDL_RenderSetLogicalSize(sdl_renderer_, w, h);
  resw_ = w; resh_ = h;
}


auto Drawer::SetClearColor(Uint8 r, Uint8 g, Uint8 b) -> void {
  clear_color_ = SDL_Color{r, g, b, 255};
}


auto Drawer::SetDrawColor(Uint8 r, Uint8 g, Uint8 b) -> void {
  SDL_SetRenderDrawColor(sdl_renderer_, r, g, b, 255);
  draw_color_ = {r, g, b, 255};
}


auto Drawer::SetDrawFont(const char* font) -> void {
  draw_font_ = font;
}


auto Drawer::DrawLine(int x1, int y1, int x2, int y2) -> void {
  SDL_RenderDrawLine(sdl_renderer_, x1, y1, x2, y2);
}

auto Drawer::DrawRect(int x1, int y1, int x2, int y2, bool fill) -> void {
  if (fill) {
    SDL_Rect draw_rect{x1, y1, x2-x1, y2-y1};
    SDL_RenderDrawRect(sdl_renderer_, &draw_rect);
    return;
  }

  DrawLine(x1, y1, x2, y1);
  DrawLine(x2, y1, x2, y2);
  DrawLine(x2, y2, x1, y2);
  DrawLine(x1, y2, x1, y1);
}

auto Drawer::DrawTriangle(int x1, int y1, int x2, int y2, int x3, int y3, bool fill) -> void {
  if (fill) {
  SDL_Vertex verts[] = {
      {{(float)x1, (float)y1}, draw_color_},
      {{(float)x2, (float)y2}, draw_color_},
      {{(float)x3, (float)y3}, draw_color_}
    };
    SDL_RenderGeometry(sdl_renderer_, NULL, verts, 3, NULL, 0);
    return;
  }

  DrawLine(x1, y1, x2, y2);
  DrawLine(x2, y2, x3, y3);
  DrawLine(x3, y3, x1, y1);
}

auto Drawer::DrawTriangleTextured(int x1, int y1,
                                  int x2, int y2,
                                  int x3, int y3,
                                  float u1, float v1,
                                  float u2, float v2,
                                  float u3, float v3,
                                  const char* name) -> void {
  SDL_Vertex verts[] = {
      {{(float)x1, (float)y1}, draw_color_, {u1, v1}},
      {{(float)x2, (float)y2}, draw_color_, {u2, v2}},
      {{(float)x3, (float)y3}, draw_color_, {u3, v3}}
    };
    SDL_RenderGeometry(
      sdl_renderer_,
      sprites_[name]->texture,
      verts,
      3, NULL, 0);
}


auto Drawer::DrawText(int x, int y, const char* text,
    float xscale, float yscale) -> void {
  SDL_Rect texture_rect{0, 0};
  SDL_Texture *texture = RenderText(text, &texture_rect);
  SDL_Rect draw_rect{
    x, y,
    int(texture_rect.w * xscale),
    int(texture_rect.h * yscale)};

  SDL_RenderCopy(
    sdl_renderer_, texture,
    &texture_rect, &draw_rect);

  SDL_DestroyTexture(texture);
}


auto Drawer::DrawTextEx(int x, int y, const char* text,
    float xscale, float yscale,
    float angle, 
    int h_align, int v_align) -> void {
  SDL_Rect texture_rect{0, 0};
  SDL_Texture *texture = RenderText(text, &texture_rect);
  SDL_Rect draw_rect{
    x, y,
    int(texture_rect.w * xscale),
    int(texture_rect.h * yscale)};
  SDL_Point center{texture_rect.w / 2 * h_align, texture_rect.h / 2 * v_align};

  SDL_RenderCopyEx(
    sdl_renderer_, texture,
    &texture_rect, &draw_rect,
    angle, &center,
    SDL_FLIP_NONE);

  SDL_DestroyTexture(texture);
}


auto Drawer::DrawSprite(int x, int y, const char* name,
    float xscale, float yscale) -> void {
  Sprite *spr = sprites_[name];
  SDL_Rect draw_rect{
    x, y,
    int(spr->w * xscale), int(spr->h * yscale)};

  SDL_RenderCopy(
    sdl_renderer_, spr->texture,
    &spr->rect, &draw_rect);
}

auto Drawer::DrawSpriteEx(int x, int y, const char* name,
    float xscale, float yscale, float angle,
    int h_align, int v_align) -> void {
  Sprite *spr = sprites_[name];
  SDL_Rect draw_rect{
    x, y,
    int(spr->w * xscale), int(spr->h * yscale)};
  SDL_Point center{spr->w / 2 * h_align, spr->h / 2 * v_align};

  SDL_RenderCopyEx(
    sdl_renderer_, spr->texture,
    &spr->rect, &draw_rect,
    angle, &center,
    SDL_FLIP_NONE);
}


// Internal methods
// ~ Draw
auto Drawer::RenderText(const char* text, SDL_Rect* rect) -> SDL_Texture* {
  SDL_Color draw_color;
  SDL_GetRenderDrawColor(
    sdl_renderer_,
    &draw_color.r,
    &draw_color.g,
    &draw_color.b,
    &draw_color.a);

  SDL_Surface *textsurf = TTF_RenderUTF8_Solid(
                            fonts_[draw_font_],
                            text,
                            draw_color);

  SDL_Texture *texture = SDL_CreateTextureFromSurface(sdl_renderer_, textsurf);
  rect->w = textsurf->w;
  rect->h = textsurf->h;

  SDL_FreeSurface(textsurf);

  return texture;
}

}
