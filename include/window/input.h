#pragma once

#include <stdexcept>

#include <SDL.h>

namespace eng::window {

class Input {
public:
  // Static methods
  static auto Init()        -> void;

  static auto GetInstance() -> Input*;

  static auto Quit()        -> void;

  // Methods
  // ~ Main
  auto Update() -> void;

  // ~ Keyboard input
  auto IsKeyDown(SDL_Scancode)      const -> bool;

  auto IsKeyPressed(SDL_Scancode)   const -> bool;

  auto IsKeyReleased(SDL_Scancode)  const -> bool;

  // ~ Mouse input
  auto IsMouseButtonDown(int)       const -> bool;

  auto IsMouseButtonPressed(int)    const -> bool;

  auto IsMouseButtonReleased(int)   const -> bool;

  auto GetMousePosition(int*, int*) const -> void;

  auto GetMouseDelta(int*, int*)    const -> void;

  auto SetCursorLock(bool)                -> void;

private:
  Input();
  
  // Vars
  SDL_Window* sdl_window_ = nullptr;

  const Uint8* keyboard_state_ = nullptr;
  Uint8 previous_keyboard_state_[264];

  Uint32 mouse_state_ = 0;
  int mousex_ = 0;
  int mousey_ = 0;

  Uint32 previous_mouse_state_ = 0;
  int previous_mousex_ = 0;
  int previous_mousey_ = 0;

  bool is_cursor_locked_ = false;

  static Input* kInstance;

};

}
