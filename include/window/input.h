#pragma once

#include <SDL.h>
#include <SDL_scancode.h>

namespace eng::sdl {

class Input {
public:
  Input(SDL_Window*);

  // Not copyable
  Input(const Input&) = delete;

  Input operator=(const Input&) = delete;

  // Not movable
  Input(Input&&) = delete;

  Input& operator=(Input&&) = delete;

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

};

}
