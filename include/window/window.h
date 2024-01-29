#pragma once
#include <SDL.h>

#include "window/input.h"
#include "window/audio.h"
#include "window/drawer.h"

namespace eng::window {

class Window {
public:
  // - Static methods -
  // Creates the window instance. If it is alredy opened,
  // throws an exception.
  static auto Create(const char*, int, int) -> void;

  // Returns a pointer to a window. If window doesn't exist,
  // throws an exception.
  static auto GetInstance() -> Window*;

  // Destroys the window instance. If window doesn't exist,
  // throws an exception.
  static auto Destroy()     -> void;

  // ! Not copyable
  Window(const Window&) = delete;
  Window operator=(const Window&) = delete;

  // ! Not movable
  Window(Window&&) = delete;
  Window& operator=(Window&&) = delete;

  // - Methods -
  // Returns a pointer to the SDL window instance.
  auto GetSDLWindowInstance() const -> SDL_Window*;

  // Returns an unsigned 32-bit value representing
  // the number of milliseconds since the SDL library initialized.
  auto GetTicks()             const -> Uint32;

  // Assigns window pixel size to passed pointers.
  auto GetSize(int*, int*)    const -> void;

  // Returns the value of the variable "is_close_requested"
  // that assigns in the "UpdateSurface" method.
  // It sets to true if SDL_QUIT event polled.
  auto IsCloseRequested()     const -> bool;

  // Polls SDL events and updates input system.
  auto PollEvents()                 -> void;

  // Calls drawer "Present" method that updates window surface,
  // showing the rendered frame.
  auto UpdateSurface()              -> void;

private:
  // - Internal static methods -
  // Creates ang gets instances of all subsystems:
  // [input system], [audio system] and [drawer]
  auto static InitSubsytems()       -> void;
  
  // Contrstructor
  explicit Window (const char*, int, int);

  // Destructor
  ~Window();

  // Vars
  Input* input_;
  Audio* audio_;
  Drawer* drawer_;

  SDL_Window *sdl_window_;

  int width_;
  int height_;

  bool is_fullscreen_      = false;
  bool is_close_requested_ = false;
  bool is_cursor_locked_   = false;

  static Window* kInstance;
};

}
