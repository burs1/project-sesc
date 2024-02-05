#include <SDL.h>

#include "window/drawer-sdl.h"
#include "logger/logger.h"

namespace eng::window {

DrawerSDL::DrawerSDL(SDL_Window* sdl_window)
    : sdl_window_(sdl_window) 
{
  // Initialize SDL_ttf
  if (TTF_Init() < 0) 
    throw std::runtime_error(TTF_GetError());

  // Init IMG
  if (IMG_Init(IMG_INIT_PNG) < 0) 
    throw std::runtime_error(IMG_GetError());

  // Create renderer
  sdl_renderer_ = SDL_CreateRenderer(
                    sdl_window_,
                    -1,
                    SDL_RENDERER_ACCELERATED);

  // Check if renderer was successfully created
  if (sdl_renderer_ == NULL) 
    throw std::runtime_error(SDL_GetError());
}


DrawerSDL::~DrawerSDL() 
{
  // Unload all loaded fonts
  for (auto [name, font] : fonts_) 
    TTF_CloseFont(font);

  // Unload all loaded sprites
  for (auto [name, texture] : textures_) 
    delete texture;

  SDL_DestroyRenderer(sdl_renderer_);
  TTF_Quit();
  IMG_Quit();
}


// - Methods -
auto DrawerSDL::Present() -> void 
{
  SDL_RenderPresent(sdl_renderer_);

  auto& [cr, cg, cb, ca] = clear_color_;
  SDL_SetRenderDrawColor(sdl_renderer_, cr, cg, cb, ca);

  SDL_RenderClear(sdl_renderer_);

  auto& [dr, dg, db, da] = draw_color_;
  SDL_SetRenderDrawColor(sdl_renderer_, dr, dg, db, da);

  log::Info("Drawer: frame presented");
}


auto DrawerSDL::LoadTexture(const char *file, const char *name) -> void 
{
  if (textures_.contains(name)) 
    throw std::runtime_error("Sprite with name \"" + std::string(name) + "\" already exists");

  SDL_Texture *texture = IMG_LoadTexture(sdl_renderer_, file);
  
  // Check if texture was successfuly loaded
  if (texture == NULL) 
    throw std::runtime_error(SDL_GetError());

  textures_[name] = new Texture(texture);
}


auto DrawerSDL::UnloadTexture(const char *name) -> void {
  delete textures_[name]; textures_.erase(name);
}


auto DrawerSDL::LoadFont(const char *file, const char *name, int size) -> void
{
  if (fonts_.contains(name)) 
    throw std::runtime_error("Font with name \"" + std::string(name) + "\" already has been loaded");

  fonts_[name] = TTF_OpenFont(file, size);

  // Check if font was successfully loaded
  if (fonts_[name] == NULL) 
    throw std::runtime_error(TTF_GetError());
}


auto DrawerSDL::UnloadFont(const char *name) -> void { 
  TTF_CloseFont(fonts_[name]); fonts_.erase(name);
}


auto DrawerSDL::SetClearColor(Uint8 r, Uint8 g, Uint8 b) -> void {
  clear_color_ = SDL_Color{r, g, b, 255};
}


auto DrawerSDL::SetDrawColor(Uint8 r, Uint8 g, Uint8 b) -> void {
  SDL_SetRenderDrawColor(sdl_renderer_, r, g, b, 255);
  draw_color_ = {r, g, b, 255};
}


auto DrawerSDL::SetDrawFont(const char *name) -> void {
  draw_font_ = name;
}


auto DrawerSDL::GetResolution(int *width, int *height) -> void {
  SDL_GetWindowSize(sdl_window_, width, height);
}


auto DrawerSDL::GetAspectRatio() -> float 
{
  int width, height;
  SDL_GetWindowSize(sdl_window_, &width, &height);
  return (float)width / height;
}


auto DrawerSDL::GetTexture(const char *name) -> Texture* {
  return textures_[name];
}


// ~ Draw
auto DrawerSDL::DrawPoint(int x, int y) -> void {
  SDL_RenderDrawPoint(sdl_renderer_, x, y);
}

auto DrawerSDL::DrawLine(int x1, int y1, int x2, int y2) -> void {
  SDL_RenderDrawLine(sdl_renderer_, x1, y1, x2, y2);
}


auto DrawerSDL::DrawTriangle(int x1, int y1, int x2, int y2,
                             int x3, int y3, bool fill) -> void 
{
  // Draw unfilled triangle
  if (not fill) {
    DrawLine(x1, y1, x2, y2);
    DrawLine(x2, y2, x3, y3);
    DrawLine(x3, y3, x1, y1);
    return;
  }

  // Draw filled triangle
  SDL_Vertex verts[] = {
    {{(float)x1, (float)y1}, draw_color_},
    {{(float)x2, (float)y2}, draw_color_},
    {{(float)x3, (float)y3}, draw_color_}};
  SDL_RenderGeometry(sdl_renderer_, NULL, verts, 3, NULL, 0);
}


auto DrawerSDL::DrawTriangleTextured(
    int x1, int y1,
    int x2, int y2,
    int x3, int y3,
    float u1, float v1,
    float u2, float v2,
    float u3, float v3,
    Texture *texture) -> void 
{
  SDL_Vertex verts[] = {
    {{(float)x1, (float)y1}, draw_color_, {u1, v1}},
    {{(float)x2, (float)y2}, draw_color_, {u2, v2}},
    {{(float)x3, (float)y3}, draw_color_, {u3, v3}}};
  SDL_RenderGeometry(
    sdl_renderer_,
    texture->GetTexture(),
    verts,
    3, NULL, 0);
}


auto DrawerSDL::DrawRect(int x1, int y1, int x2, int y2, bool fill) -> void 
{
  // Draw filled rect
  if (fill) {
    SDL_Rect draw_rect{x1, y1, x2-x1, y2-y1};
    SDL_RenderDrawRect(sdl_renderer_, &draw_rect);
    return;
  }

  // Draw unfilled rect
  DrawLine(x1, y1, x2, y1);
  DrawLine(x2, y1, x2, y2);
  DrawLine(x2, y2, x1, y2);
  DrawLine(x1, y2, x1, y1);
}


auto DrawerSDL::DrawTexture(int x, int y, const char *name,
                            float xscale, float yscale) -> void 
{
  Texture* texture = textures_[name];
  SDL_Rect dsrect;
  texture->GetSize(&dsrect.w, &dsrect.h);

  SDL_Rect srcrect{
    x, y,
    int(dsrect.w * xscale),
    int(dsrect.h * yscale)};

  SDL_RenderCopy(sdl_renderer_, texture->GetTexture(), &dsrect, &srcrect);
}

auto DrawerSDL::DrawTextureEx(
    int x, int y, const char *name,
    float xscale, float yscale, float angle,
    int h_align, int v_align) -> void 
{
  Texture* texture = textures_[name];
  SDL_Rect dsrect;
  texture->GetSize(&dsrect.w, &dsrect.h);

  SDL_Rect srcrect{
    x, y,
    int(dsrect.w * xscale),
    int(dsrect.h * yscale)};
  SDL_Point center{dsrect.w / 2 * h_align, dsrect.h / 2 * v_align};

  SDL_RenderCopyEx(
    sdl_renderer_,
    texture->GetTexture(),
    &dsrect, &srcrect,
    angle, &center,
    SDL_FLIP_NONE);
}


auto DrawerSDL::DrawText(
    int x, int y, const std::string &text,
    float xscale, float yscale) -> void 
{
  SDL_Rect dsrect{0, 0};
  SDL_Texture *texture = RenderTextSDL(text.c_str(), draw_font_, &dsrect);
  SDL_Rect srcrect{
    x, y,
    int(dsrect.w * xscale),
    int(dsrect.h * yscale)};

  SDL_RenderCopy(sdl_renderer_, texture, &dsrect, &srcrect);
  SDL_DestroyTexture(texture);
}


auto DrawerSDL::DrawTextEx(
    int x, int y, const std::string &text,
    float xscale, float yscale, float angle, 
    int h_align, int v_align) -> void 
{
  SDL_Rect dsrect{0, 0};
  SDL_Texture *texture = RenderTextSDL(text.c_str(), draw_font_, &dsrect);
  SDL_Rect srcrect{
    x, y,
    int(dsrect.w * xscale),
    int(dsrect.h * yscale)};
  SDL_Point center{dsrect.w / 2 * h_align, dsrect.h / 2 * v_align};

  SDL_RenderCopyEx(
    sdl_renderer_,
    texture,
    &dsrect, &srcrect,
    angle, &center,
    SDL_FLIP_NONE);
  SDL_DestroyTexture(texture);
}


// ~ Render
auto DrawerSDL::RenderText(const std::string &text,
                           const char *font) -> Texture* 
{
  SDL_Rect rect;
  SDL_Texture* texture = RenderTextSDL(
                          text.c_str(),
                          (font[0] == '\0' ? draw_font_ : font),
                          &rect);
  return new Texture(texture, rect);
}

// Internal methods
// ~ Render
auto DrawerSDL::RenderTextSDL(const char *text, const char *font_name,
                              SDL_Rect* rect) -> SDL_Texture* 
{
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
