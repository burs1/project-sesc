#include <stdexcept>

#include <SDL.h>

#include "window/input.h"
#include "window/audio.h"
#include "window/window.h"
#include "window/drawer-sdl.h"
#include "logger/logger.h"

namespace eng::window {

// Constructor
Window::Window (const char *title, int width, int height)
    : width_(width), height_(height) {
  InitSDL();
  CreateSDLWindow(title, width, height);

  // Initialise subsystems
  input_ = new Input(sdl_window_);
  audio_ = new Audio();
  drawer_ = new DrawerSDL(sdl_window_);

  log::Info("Window created");
}


// Destructor
Window::~Window() {
  // Destroy systems
  delete input_;
  delete audio_;
  delete drawer_;

  SDL_DestroyWindow(sdl_window_);
  log::Info("SDL_Window destroyed");

  SDL_Quit();
  log::Info("SDL_main quit");

  log::Info("Window destroyed");
}


// - Methods -
auto Window::GetSDLWindow() const -> SDL_Window* {
  return sdl_window_;
}


auto Window::GetInput() const -> Input* {
  return input_;
}


auto Window::GetAudio() const -> Audio* {
  return audio_;
}


auto Window::GetDrawer() const -> Drawer* {
  return drawer_;
}


auto Window::GetTicks() const -> Uint32 {
  return SDL_GetTicks();
}


auto Window::GetSize(int* width, int* height) const -> void {
  *width  = width_;
  *height = height_;
}


auto Window::IsCloseRequested() const -> bool {
  return is_close_requested_;
}


auto Window::PollEvents() -> void {
  SDL_Event e;
  // Poll SDL window events
  while(SDL_PollEvent(&e)) {
    if (e.type == SDL_QUIT) {
      log::Info("Window close requested");
      is_close_requested_ = true;
    }
  }

  input_->Update();
}


auto Window::UpdateSurface() -> void {
  drawer_->Present();
}


// - Internal statis methods -
auto Window::InitSDL() -> void {
  // Init SDL
  if (SDL_Init(SDL_INIT_EVERYTHING) < 0) {
    throw std::runtime_error( SDL_GetError() );
  }

  log::Info("SDL_main initialised");
}


auto Window::CreateSDLWindow(const char* title, int width, int height) -> void {
  // Create window
  sdl_window_ = SDL_CreateWindow(
    title,
    SDL_WINDOWPOS_CENTERED,
    SDL_WINDOWPOS_CENTERED,
    width, height,
    SDL_WINDOW_SHOWN
  );

  // Check if window was successfully instantiated
  if (sdl_window_ == NULL) {
    throw std::runtime_error( SDL_GetError() );
  }

  log::Info("SDL_Window created");
}


}
