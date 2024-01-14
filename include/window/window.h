#pragma once

#include <map>
#include <string>
#include <stdexcept>

#include <SDL.h>

#include "window/input.h"
#include "window/audio.h"
#include "window/drawer.h"

namespace eng::sdl {

class Window {
public:
  // Static methods
  static auto Init(const char*, int, int, bool fullscreen=false) -> void;

  static auto GetInstance() -> Window*;

  static auto Quit()        -> void;

  // Not copyable
  Window(const Window&) = delete;

  Window operator=(const Window&) = delete;

  // Not movable
  Window(Window&&) = delete;

  Window& operator=(Window&&) = delete;

  // Methods
  // ~ Main
  auto UpdateEvents()         -> void;

  auto UpdateSurface()        -> void;

  // ~ Window
  auto ToggleFullscreen()     -> void;

  auto GetTicks()             -> Uint32;

  auto SetCursorLock(bool)    -> void;

  // ~ SDL
  auto GetSDLWindowInstance() -> SDL_Window*;
  
  const bool &is_close_requested;
  const int &width, &height;

  Input* input   = nullptr;
  Audio* audio   = nullptr;
  Drawer* drawer = nullptr;

private:
  explicit Window (const char*, int, int, bool);

  ~Window();

  // Vars
  SDL_Window *sdl_window_ = nullptr;

  int width_  = 0;
  int height_ = 0;

  bool is_fullscreen_      = false;
  bool is_close_requested_ = false;
  bool is_cursor_locked_   = false;

  static Window* kInstance;
};

}
