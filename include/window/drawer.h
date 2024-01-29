#pragma once

#include <stdexcept>
#include <type_traits>

#include <SDL.h>
#include <SDL_ttf.h>
#include <SDl_image.h>

#include "window/texture.h"
#include "logger/logger.h"

namespace eng::window {

class Drawer {
public:
  // - Static methods -
  template<typename T>
  static auto Create() -> void {
    static_assert(std::is_base_of<Drawer, T>(), "Class isn't derrived from Drawer.");
    if (not kInstance) {
      kInstance = new T();
      log::Info("Drawer instance created");
      return;
    }

    throw std::runtime_error("Trying to create a drawer instance when it is already created");
  }

  static auto GetInstance()     -> Drawer*;

  static auto Destroy()         -> void;

  // - Methods -
  // Updates window surface showing the rendered frame.
  virtual auto Present()                               -> void = 0;

  // Load the file into a Texture object and inserts it in map by given name.
  virtual auto LoadTexture(const char*, const char*)   -> void = 0;

  // Unload the Texture object by it's name.
  virtual auto UnloadTexture(const char*)              -> void = 0;

  // Load the file into a Font object and inserts it in map by given name.
  virtual auto LoadFont(const char*, const char*, int) -> void = 0;

  // Unload the Font object by it's name.
  virtual auto UnloadFont(const char*)                 -> void = 0;

  // Sets frame clear color.
  virtual auto SetClearColor(Uint8, Uint8, Uint8)      -> void = 0;

  // Sets current style draw color.
  virtual auto SetDrawColor(Uint8, Uint8, Uint8)       -> void = 0;

  // Sets current style font.
  virtual auto SetDrawFont(const char*)                -> void = 0;

  // Assigns frame pixel size to passed pointers.
  virtual auto GetResolution(int*, int*)               -> void = 0;

  // Returns the ratio of frame width to height.
  virtual auto GetAspectRatio()                        -> float = 0;

  // Return pointer to the Texture object.
  virtual auto GetTexture(const char*)                 -> Texture* = 0;

  // Draws a point at the given point in the frame.
  virtual auto DrawPoint(int, int)                     -> void = 0;

  // Draws a line between given two points in the frame.
  virtual auto DrawLine(int, int, int, int)            -> void = 0;

  // Draws a triangle between the given three pixel points.
  // If fill is true, makes triangle filled with the current style
  // draw color.
  virtual auto DrawTriangle(
    int, int, 
    int, int,
    int, int,
    bool fill=true) -> void = 0;

  // Draws a textured triangle between the given three pixel points. 
  // UV coordinates define normalized coordinates of the verticies
  // of the applied triangle texture fragment.
  virtual auto DrawTriangleTextured(
    int, int,
    int, int,
    int, int,
    float, float,
    float, float,
    float, float,
    Texture*) -> void = 0;

  // Draws a rect between the given two pixel points. 
  // If fill is true, makes rect filled with the current style
  // draw color.
  virtual auto DrawRect(int, int, int, int, bool fill=true)     -> void = 0;

  // Draws a Texture object specified by name at the given
  // point with the given scale.
  virtual auto DrawTexture(int, int, const char*, float, float) -> void = 0;
  
  // Draws a Texture object specified by name at the given
  // point with the given scale rotated by specified angle.
  // Horizontal align can be set with 0 (left), 1 (center) or 2 (right).
  // Vertical align can be set with 0 (up), 1 (middle) or 2 (down).
  virtual auto DrawTextureEx(
    int, int,
    const char*,
    float, float, float,
    int halign=0, int valign=0) -> void = 0;

  // Draws a rendered text Texture object at the given
  // point with the given scale.
  virtual auto DrawText(int, int, const char*, float, float) -> void = 0;

  // Draw a rendered text Texture object at the given
  // point with the given scale rotated by specified angle.
  // Horizontal align can be set with 0 (left), 1 (center) or 2 (right).
  // Vertical align can be set with 0 (up), 1 (middle) or 2 (down).
  virtual auto DrawTextEx(
    int, int,
    const char*,
    float, float, float, 
    int halign=0, int valign=0) -> void = 0;

  // Returns Texture object that contains texture of rendered text.
  // If font wasn't specified the text will be rendered using current
  // style draw font.
  virtual auto RenderText(const char*, const char* font="") -> Texture* = 0;

protected:
  Drawer() = default;

  virtual ~Drawer() = default;

private:
  // Vars
  static Drawer* kInstance;

};


}
