#pragma once

#include <SDL.h>

#include "window/input.h"
#include "window/audio.h"
#include "window/drawer.h"

namespace eng::app {

class App; // forward declaration to make "App"
           // a friend class of Window, so nobody else
           // couldn't instantiate it.

}

namespace eng::window {

class Window {
friend app::App;
 public:
  // ! Not copyable
  Window(const Window&) = delete;
  Window operator=(const Window&) = delete;

  // ! Not movable
  Window(Window&&) = delete;
  Window& operator=(Window&&) = delete;

  // Polls SDL events and updates input system.
  auto PollEvents()                 -> void;

  // Calls drawer "Present" method that updates window surface,
  // showing the rendered frame.
  auto UpdateSurface()              -> void;

  // Returns a pointer to the instnace of SDL_Window.
  auto GetSDLWindow()         const -> SDL_Window*;

  // Returns a pointer to the instance of Input.
  auto GetInput()             const -> Input*;

  // Return a pointer to the instance of Audio.
  auto GetAudio()             const -> Audio*;

  // Returns a pointer to the instance of Drawer.
  auto GetDrawer()            const -> Drawer*;

  // Returns an unsigned 32-bit value representing
  // the number of milliseconds since the SDL library initialized.
  auto GetTicks()             const -> Uint32;

  // Assigns window's width and height in pixels to passed pointers.
  auto GetSize(int*, int*)    const -> void;

  // Returns the value of the variable "is_close_requested"
  // that assigns in the "UpdateSurface" method.
  // It sets to true if SDL_QUIT event polled.
  auto IsCloseRequested()     const -> bool;

  Input* input_;
  Audio* audio_;
  Drawer* drawer_;

 private:
  explicit Window (const char*, int, int);
  ~Window();

  // Init SDL library. If SDL wasn't successfuly initialized
  // throws an exception.
  auto InitSDL()                              -> void;

  // Creates an instance of SDL_Window object. If SDL_Window
  // wasn't successfully initialized throws an exception.
  auto CreateSDLWindow(const char*, int, int) -> void;

  SDL_Window *sdl_window_;

  int width_, height_;
  bool is_fullscreen_      = false;
  bool is_close_requested_ = false;
};

}

