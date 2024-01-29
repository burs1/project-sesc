#include <iostream>
#include <stdexcept>

#include <SDL.h>

#include "window/input.h"
#include "window/audio.h"
#include "window/drawer.h"
#include "window/window.h"
#include "window/drawer-sdl.h"
#include "logger/logger.h"

namespace eng::window {

Window* Window::kInstance = nullptr;

// - Static methods -
auto Window::Create(const char* name, int width, int height) -> void {
  if (not kInstance) {
    kInstance = new Window(name, width, height);
    log::Info("Window instance created");
    InitSubsytems();
    return;
  }

  throw std::runtime_error("Trying to create a window when it is already open");
}


auto Window::GetInstance() -> Window* {
  if (kInstance) { return kInstance; }

  throw std::runtime_error("Trying to get a window instance when it doesn't exist");
}


auto Window::Destroy() -> void {
  if (kInstance) {
    delete kInstance;
    log::Info("Window instance destroyed");
    return;
  }

  throw std::runtime_error("Trying to destroy a window when it doens't exist");
}


// - Methods -
auto Window::GetSDLWindowInstance() const -> SDL_Window* {
  return sdl_window_;
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


// Constructor
Window::Window (const char *title, int width, int height)
    : width_(width), height_(height) {
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
    SDL_WINDOW_SHOWN
  );

  if (sdl_window_ == NULL) {
    throw std::runtime_error(SDL_GetError());
  }
}

// - Internal statis methods -
auto Window::InitSubsytems() -> void {
  Input::Create();
  Audio::Create();
  Drawer::Create<DrawerSDL>();

  Window* window = Window::GetInstance();
  window->input_ = Input::GetInstance();
  window->audio_ = Audio::GetInstance();
  window->drawer_ = Drawer::GetInstance();
}


// Destructor
Window::~Window() {
  // Destroy systems
  Input::Destroy();
  Audio::Destroy();
  Drawer::Destroy();

  SDL_DestroyWindow(sdl_window_);

  SDL_Quit();
}


}
