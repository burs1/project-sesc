#include "window.h"

#include <SDL_error.h>
#include <SDL_stdinc.h>
#include <iostream>
#include <sdl_ttf.h>
#include <stdexcept>

using namespace std;
using namespace engine::window;

namespace engine {
  Window::Window (const char *title, int resw, int resh, int winw, int winh, bool fullscreen)
    : _fullscreen(fullscreen), isOpened(_isOpened),
    _w(winw), _h(winh), w(_w), h(_h),
    _resw(resw), _resh(resh), resw(_resw), resh(_resh) {
    // Init SDL
    if (SDL_Init(SDL_INIT_EVERYTHING) < 0)
      throw runtime_error(SDL_GetError());

    // Init TTF
    if (TTF_Init() < 0)
      throw runtime_error(TTF_GetError());

    // Init IMG
    if (IMG_Init(IMG_INIT_PNG) < 0)
      throw runtime_error(IMG_GetError());

    // Create window
    _window = SDL_CreateWindow(
      title, 
      SDL_WINDOWPOS_CENTERED, 
      SDL_WINDOWPOS_CENTERED, 
      _w, _h, 
      _fullscreen ? SDL_WINDOW_FULLSCREEN : SDL_WINDOW_SHOWN
    );

    if (_window == NULL)
      throw runtime_error(SDL_GetError());

    // Create renderer
    _renderer = SDL_CreateRenderer(
      _window, -1, SDL_RENDERER_ACCELERATED
    );

    if (_renderer == NULL)
      throw runtime_error(SDL_GetError());

    SDL_RenderSetLogicalSize(_renderer, resw, resh);
  }

  Window::~Window() {
    // Unload all textures
    for (auto [name, spr] : _sprites) { delete spr; }
    _sprites.clear();

    // Unload all fonts
    for (auto [name, fnt] : _fonts) { TTF_CloseFont(fnt); }
    _fonts.clear();

    SDL_DestroyRenderer(_renderer);
    SDL_DestroyWindow(_window);

    IMG_Quit();
    TTF_Quit();
    SDL_Quit();
  }

  // methods
  // ~ main
  auto Window::update_events() -> void {
    SDL_Event e;
    while(SDL_PollEvent(&e)) {
      if (e.type == SDL_QUIT) { _isOpened = false; }
    }

    _inputs = SDL_GetKeyboardState(NULL);
  }

  auto Window::update_surface() -> void {
    SDL_RenderPresent(_renderer);

    // Clear renderer
    SDL_SetRenderDrawColor(_renderer, 0, 0, 0, 255);
    SDL_RenderClear(_renderer);

    // Set draw color back
    SDL_SetRenderDrawColor(_renderer,
      _drawColor.r,
      _drawColor.g,
      _drawColor.b,
      _drawColor.a
    );
  }

  // ~ user
  auto Window::get_ticks() -> Uint32 {
    return SDL_GetTicks();
  }

  // ~ input
  auto Window::input_check(SDL_Scancode scancode) -> bool {
    return _inputs[scancode];
  }

  auto Window::input_axis(SDL_Scancode l, SDL_Scancode r) -> int {
    return _inputs[r] - _inputs[l];
  }


  
  // ~ window
  auto Window::toggle_fullscreen() -> void {
    _fullscreen = !_fullscreen;
    SDL_SetWindowFullscreen(_window, _fullscreen ? SDL_WINDOW_FULLSCREEN : SDL_WINDOW_SHOWN);
  }

  // ~ resources
  auto Window::load_sprite(const char *path, const char *name) -> void {
    // Check if sprite with such name alredy loaded
    if (_sprites.contains(name))
      throw runtime_error("Sprite with name \"" + string(name) + "\" already exists");

    SDL_Texture *texture = IMG_LoadTexture(_renderer, path);

    // Check if texture was successfuly loaded
    if (not texture)
      throw runtime_error( SDL_GetError() );

    _sprites[name] = new Sprite(texture);
  }

  auto Window::load_font(const char *path, int size, const char *name) -> void {
    // Check if sprite with such name alredy loaded
    if (_fonts.contains(name))
      throw runtime_error("Fonst with name \"" + string(name) + "\" already exists");

    TTF_Font *font = TTF_OpenFont(path, size);

    // Check if font was successfuly loaded
    if (not font)
      throw runtime_error( SDL_GetError() );

    _fonts[name] = font;
  }

  // ~ draw
  auto Window::set_draw_color(Uint8 r, Uint8 g, Uint8 b, Uint8 a) -> void {
    SDL_SetRenderDrawColor(_renderer, r, g, b, a);
    _drawColor = SDL_Color{r, g, b, a};
  }
  
  auto Window::draw_line(int x1, int y1, int x2, int y2) -> void {
    SDL_RenderDrawLine(_renderer, x1, y1, x2, y2);
  }
    
  auto Window::set_font(const char* fontname) -> void {
    _curfont = fontname;
  }

  auto Window::draw_text(const char* text, int x, int y, float xscale, float yscale) -> void {
    SDL_Rect txtr_rect{0, 0};
    SDL_Texture *texture = _render_text(text, &txtr_rect);

    SDL_Rect draw_rect{
      x, y,
      int(txtr_rect.w * xscale),
      int(txtr_rect.h * yscale)
    };

    if (SDL_RenderCopy(_renderer, texture, &txtr_rect, &draw_rect) < 0)
      throw runtime_error(SDL_GetError());

    SDL_DestroyTexture(texture);
  }
 
  auto Window::draw_text_ex(const char* text, int x, int y, float xscale, float yscale, float angle) -> void {
    SDL_Rect txtr_rect{0, 0};
    SDL_Point center{0, 0};
    SDL_Texture *texture = _render_text(text, &txtr_rect);

    SDL_Rect draw_rect{
      x, y,
      int(txtr_rect.w * xscale),
      int(txtr_rect.h * yscale)
    };

    if (SDL_RenderCopyEx(_renderer, texture, &txtr_rect, &draw_rect, angle, &center, SDL_FLIP_NONE) < 0)
      throw runtime_error(SDL_GetError());
    SDL_DestroyTexture(texture);
  }
 
  auto Window::draw_sprite(const char* sprname, int x, int y, float xscale, float yscale) -> void {
    Sprite *spr = _sprites[sprname];
    SDL_Rect draw_rect{x, y, int(spr->w * xscale), int(spr->h * yscale)};

    if (SDL_RenderCopy(_renderer, spr->texture, &spr->rect, &draw_rect) < 0)
      throw runtime_error( SDL_GetError() );
  }

  auto Window::draw_sprite_ex(const char* sprname, int x, int y, float xscale, float yscale, float angle, int xcent, int ycent) -> void {
    Sprite *spr = _sprites[sprname];
    SDL_Rect draw_rect{x, y, int(spr->w * xscale), int(spr->h * yscale)};
    SDL_Point center{xcent, ycent};

    if (SDL_RenderCopyEx(_renderer, spr->texture, &spr->rect, &draw_rect, angle, &center, SDL_FLIP_NONE) < 0)
      throw runtime_error( SDL_GetError() );
  }

  // internal methods
  // ~ draw
  auto Window::_render_text(const char *text, SDL_Rect *rect) -> SDL_Texture* {
    SDL_Surface *textsurf = TTF_RenderText_Solid(_fonts[_curfont], text, _drawColor);

    // Check if surface successfuly created
    if (not textsurf)
      throw runtime_error( TTF_GetError() );

    SDL_Texture *texture = SDL_CreateTextureFromSurface(_renderer, textsurf);

    // Check if texture successfuly created
    if (not texture)
      throw runtime_error( SDL_GetError() );
    
    rect->w = textsurf->w;
    rect->h = textsurf->h;

    SDL_FreeSurface(textsurf);

    return texture;
  }
}
