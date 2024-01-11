#include "window/input.h"

namespace eng::sdl {

Input::Input(SDL_Window* sdl_window)
  : sdl_window_(sdl_window) {
  // Read keyboard and mouse states
  keyboard_state_ = SDL_GetKeyboardState(NULL);
  mouse_state_ = SDL_GetMouseState(&mousex_, &mousey_);
}


// Methods
// ~ Main
void Input::Update() {
  // Copy previous states
  for (int i = 0; i < 264; ++i) {
    previous_keyboard_state_[i] = keyboard_state_[i];
  }

  previous_mouse_state_ = mouse_state_;
  previous_mousex_ = mousex_;
  previous_mousey_ = mousey_;

  // Read current keyboard and mouse states
  keyboard_state_ = SDL_GetKeyboardState(NULL);
  mouse_state_ = SDL_GetMouseState(&mousex_, &mousey_);

  if (is_cursor_locked_) {
    SDL_WarpMouseInWindow(sdl_window_, 0, 0);
  }
}


// ~ Keyboard input
bool Input::IsKeyDown(SDL_Scancode key) const {
  return keyboard_state_[ key ];
}


bool Input::IsKeyPressed(SDL_Scancode key) const {
  return keyboard_state_[ key ];
}


bool Input::IsKeyReleased(SDL_Scancode key) const {
  return keyboard_state_[ key ];
}


// ~ Mouse input
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
  if (is_cursor_locked_) {
    *x = mousex_;
    *y = mousey_;
    return;
  }

  *x = mousex_ - previous_mousex_;
  *y = mousey_ - previous_mousey_;
}


auto Input::SetCursorLock(bool value) -> void {
  is_cursor_locked_ = value;
}

}
