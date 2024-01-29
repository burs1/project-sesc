#include <stdexcept>

#include "window/input.h"
#include "window/window.h"
#include "logger/logger.h"

namespace eng::window {

Input* Input::kInstance = nullptr;

// - Static methods -
auto Input::Create() -> void {
  if (not kInstance) {
    kInstance = new Input();
    log::Info("Input system instance created");
    return;
  }

  throw std::runtime_error("Trying to create an input system instance when it is already open");
}


auto Input::GetInstance() -> Input* {
  if (kInstance) { return kInstance; }
  
  throw std::runtime_error("Trying to get an input system instance when it doesn't exist");
}


auto Input::Destroy() -> void {
  if (kInstance) {
    delete kInstance;
    log::Info("Input system instance destroyed");
    return;
  }

  throw std::runtime_error("Trying to destroy an input system instance when it doens't exist");
}


// - Methods -
void Input::Update() {
  // Copy previous states
  for (int i = 0; i < 264; ++i) {
    previous_keyboard_state_[i] = keyboard_state_[i];
  }

  previous_mouse_state_ = mouse_state_;

  if (is_cursor_locked_) {
    int w, h;
    SDL_GetWindowSize(sdl_window_, &w, &h);

    SDL_WarpMouseInWindow(sdl_window_, w/2, h/2);

    previous_mousex_ = w/2;
    previous_mousey_ = h/2;
  }
  else {
    previous_mousex_ = mousex_;
    previous_mousey_ = mousey_;
  }

  // Read current keyboard and mouse states
  keyboard_state_ = SDL_GetKeyboardState(NULL);
  mouse_state_ = SDL_GetMouseState(&mousex_, &mousey_);
}


bool Input::IsKeyDown(SDL_Scancode key) const {
  return keyboard_state_[ key ];
}


bool Input::IsKeyPressed(SDL_Scancode key) const {
  return keyboard_state_[ key ];
}


bool Input::IsKeyReleased(SDL_Scancode key) const {
  return keyboard_state_[ key ];
}


bool Input::IsMouseButtonDown(int button) const {
  return SDL_BUTTON(button) & mouse_state_;
}


bool Input::IsMouseButtonPressed(int button) const {
  return (SDL_BUTTON(button) & mouse_state_) and
          not (SDL_BUTTON(button) & previous_mouse_state_);
}


bool Input::IsMouseButtonReleased(int button) const {
  return not (SDL_BUTTON(button) & mouse_state_) and
          (SDL_BUTTON(button) & previous_mouse_state_);
}


void Input::GetMousePosition(int *x, int *y) const {
  *x = mousex_;
  *y = mousey_;
}


void Input::GetMouseDelta(int *x, int *y) const {
  *x = mousex_ - previous_mousex_;
  *y = mousey_ - previous_mousey_;
}


auto Input::SetCursorLock(bool lock) -> void {
  is_cursor_locked_ = lock;
}


// Constructor
Input::Input()
  : sdl_window_( Window::GetInstance()->GetSDLWindowInstance() ) {
  // Read keyboard and mouse states
  keyboard_state_ = SDL_GetKeyboardState(NULL);
  mouse_state_ = SDL_GetMouseState(&mousex_, &mousey_);
}

}
