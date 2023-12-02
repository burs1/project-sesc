#pragma once

#include <map>
#include <sdl.h>
#include <string>
#include <cassert>
#include "sprite.h"
#include <stdexcept>
#include <sdl_ttf.h>
#include <sdl_image.h>

namespace engine {
  class Window {
  public:
    explicit Window (const char*, int, int, int, int, bool fullscreen=false);

    ~Window();

    // methods
    // ~ main
    auto update_events()                            -> void;

    auto update_surface()                           -> void;


    // ~ window
    auto toggle_fullscreen()                        -> void;


    // ~ user
    auto get_ticks()                                -> Uint32;

    // ~ input
    auto input_check(SDL_Scancode)                  -> bool;

    auto input_axis(SDL_Scancode, SDL_Scancode)     -> int;


    // ~ resources
    auto load_sprite(const char*, const char*) -> void;

    auto load_font(const char*, int size, const char*)   -> void;


    // ~ draw
    auto set_draw_color(Uint8, Uint8, Uint8, Uint8)                             -> void;

    auto draw_line(int, int, int, int)                                          -> void;

    auto set_font(const char*)                                                  -> void;

    auto draw_text(const char*, int, int, float, float)                         -> void;

    auto draw_text_ex(const char*, int, int, float, float, float)               -> void;
     
    auto draw_sprite(const char*, int, int, float, float)                       -> void;
    
    auto draw_sprite_ex(const char*, int, int, float, float, float, int, int)   -> void;


    const bool &isOpened;
    const int &w, &h, &resw, &resh;

  private:
    // internal methods
    // ~ draw
    auto _render_text(const char*, SDL_Rect*) -> SDL_Texture*;

    // vars
    SDL_Window *_window;
    SDL_Renderer *_renderer;

    const Uint8 *_inputs;

    SDL_Color _clearColor = SDL_Color{0, 0, 0, 255};
    SDL_Color _drawColor = SDL_Color{255, 255, 255, 255};

    std::map< const char*, window::Sprite* > _sprites;
    std::map< const char*, TTF_Font* > _fonts;

    const char *_curfont = "";

    int _w, _h, _resw, _resh;

    bool _fullscreen;
    bool _isOpened;
  };
}
