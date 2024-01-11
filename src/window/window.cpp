#include "window/window.h"
#include <SDL_events.h>
#include <SDL_mouse.h>

namespace eng::sdl {

Window::Window (const char *title, int width, int height, bool fullscreen)
  : is_fullscreen_(fullscreen), is_close_requested(is_close_requested_),
  width_(width), height_(height), width(width_), height(height_) {
  // Init SDL
  if (SDL_Init(SDL_INIT_EVERYTHING) < 0) {
    throw std::runtime_error(SDL_GetError());
  }

  // Create window
  sdl_window_ = SDL_CreateWindow(
    title, 
    SDL_WINDOWPOS_CENTERED, 
    SDL_WINDOWPOS_CENTERED, 
    width_, height_, 
    is_fullscreen_ ? SDL_WINDOW_FULLSCREEN : SDL_WINDOW_SHOWN
  );

  if (sdl_window_ == NULL) {
    throw std::runtime_error(SDL_GetError());
  }

  // Initialize modules
  input = new Input(sdl_window_);
  audio = new Audio;
  drawer = new Drawer(sdl_window_);
}


Window::~Window() {
  delete drawer;
  delete audio;
  delete input;

  SDL_DestroyWindow(sdl_window_);

  SDL_Quit();
}


// Methods
// ~ Main
auto Window::UpdateEvents() -> void {
  SDL_Event e;
  while(SDL_PollEvent(&e)) {
    if (e.type == SDL_QUIT) {
      is_close_requested_ = true;
    }
  }

  input->Update();
}


auto Window::UpdateSurface() -> void {
  drawer->Present();
}


// ~ Window
auto Window::ToggleFullscreen() -> void {
  is_fullscreen_ = !is_fullscreen_;
  SDL_SetWindowFullscreen(sdl_window_, is_fullscreen_ ? SDL_WINDOW_FULLSCREEN : SDL_WINDOW_SHOWN);
}


auto Window::GetTicks() -> Uint32 {
  return SDL_GetTicks();
}


auto Window::SetCursorLock(bool value) -> void {
  is_cursor_locked_ = value;
}


}
