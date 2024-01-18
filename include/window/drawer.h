#pragma once

#include <map>
#include <string>
#include <stdexcept>
#include <type_traits>

#include <SDL.h>
#include <SDL_ttf.h>
#include <SDl_image.h>

#include "window/texture.h"

namespace eng::window {

class Drawer {
public:
  // Static methods
  template<typename T>
  static auto Init() -> void {
    static_assert(std::is_base_of<Drawer, T>(), "Class isn't derrived from Drawer.");
    if (not kInstance) {
      kInstance = new T();
      return;
    }
    throw std::runtime_error("Drawer is already online.");
  }

  static auto GetInstance()     -> Drawer*;

  static auto Quit()            -> void;

  // Methods
  // ~ Main
  virtual auto Present()                               -> void = 0;

  // ~ Resources
  virtual auto LoadTexture(const char*, const char*)   -> void = 0;

  virtual auto UnloadTexture(const char*)              -> void = 0;

  virtual auto LoadFont(const char*, const char*, int) -> void = 0;

  virtual auto UnloadFont(const char*)                 -> void = 0;

  // ~ Setters
  virtual auto SetClearColor(Uint8, Uint8, Uint8)      -> void = 0;

  virtual auto SetDrawColor(Uint8, Uint8, Uint8)       -> void = 0;

  virtual auto SetDrawFont(const char*)                -> void = 0;

  // ~ Getters
  virtual auto GetResolution(int*, int*)               -> void = 0;

  virtual auto GetAspectRatio()                        -> float = 0;

  virtual auto GetTexture(const char*)                 -> Texture* = 0;

  // ~ Draw
  virtual auto DrawPoint(int, int)                     -> void = 0;

  virtual auto DrawLine(int, int, int, int)            -> void = 0;

  virtual auto DrawTriangle(
    int, int, 
    int, int,
    int, int,
    bool fill=true) -> void = 0;

  virtual auto DrawTriangleTextured(
    int, int,
    int, int,
    int, int,
    float, float,
    float, float,
    float, float,
    Texture*) -> void = 0;

  virtual auto DrawRect(int, int, int, int, bool fill=true)     -> void = 0;

  virtual auto DrawTexture(int, int, const char*, float, float) -> void = 0;
  
  virtual auto DrawTextureEx(
    int, int,
    const char*,
    float, float, float,
    int halign=0, int valign=0) -> void = 0;

  virtual auto DrawText(int, int, const char*, float, float) -> void = 0;

  virtual auto DrawTextEx(
    int, int,
    const char*,
    float, float, float, 
    int halign=0, int valign=0) -> void = 0;

  // Rendering
  virtual auto RenderText(const char*, const char* font="") -> Texture* = 0;

protected:
  Drawer() = default;

  virtual ~Drawer() = 0;

private:
  // Vars
  static Drawer* kInstance;

};


}
