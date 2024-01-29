#pragma once

#include <SDL.h>

namespace eng::window {

class Input {
public:
  // - Static methods -
  static auto Create()      -> void;

  static auto GetInstance() -> Input*;

  static auto Destroy()     -> void;

  // - Methods -
  // Read keyboard and mouse states at current frame and copies
  // states of the previous frame. If "is_cursor_locked_" is true
  // moves cursor to the center of the SDL window.
  auto Update() -> void;

  // Returns true if the key is down in the current frame,
  // else returns false.
  auto IsKeyDown(SDL_Scancode)      const -> bool;

  // Returns true if the key was up in the previous frame and
  // down in the current, else returns false.
  auto IsKeyPressed(SDL_Scancode)   const -> bool;

  // Returns true if the key was down in the previous frame and
  // up in the current, else returns false.
  auto IsKeyReleased(SDL_Scancode)  const -> bool;

  // Returns true if the mouse button down in the current frame,
  // else returns false.
  auto IsMouseButtonDown(int)       const -> bool;

  // Returns true if the mouse button was up in the previous frame
  // and down in the current, else return false.
  auto IsMouseButtonPressed(int)    const -> bool;

  // Returns true if the mouse button was down in the previous frame
  // and up in the current, else return false.
  auto IsMouseButtonReleased(int)   const -> bool;

  // Assigns relative mouse pixel position in window to passed pointers. 
  auto GetMousePosition(int*, int*) const -> void;

  // Assigns mouse pixel position delta to passed pointers.
  auto GetMouseDelta(int*, int*)    const -> void;

  // Sets "is_cursor_locked_" variable to passed value.
  auto SetCursorLock(bool)                -> void;

private:
  Input();
  
  // Vars
  SDL_Window* sdl_window_ = nullptr;

  const Uint8* keyboard_state_ = nullptr;
  Uint8 previous_keyboard_state_[264];

  Uint32 mouse_state_;
  int mousex_;
  int mousey_;

  Uint32 previous_mouse_state_;
  int previous_mousex_;
  int previous_mousey_;

  bool is_cursor_locked_ = false;

  static Input* kInstance;

};

}
