#pragma once

#include <string>

#include <SDL.h>
#include <SDL_ttf.h>
#include <SDl_image.h>

#include "window/texture.h"

namespace eng::window {

class Window; // forward declaration to make "Window"
              // a friend class of Drawer, so nobody else
              // couldn't instantiate it.

class Drawer {
friend Window;
public:
  // - Methods -
  // Updates window surface showing the rendered frame.
  virtual auto Present()                               -> void = 0;


  // Loads a file into a Texture object and inserts it in map
  // by given name.
  virtual auto LoadTexture(const char*, const char*)   -> void = 0;


  // Unloads a Texture object by it's name.
  virtual auto UnloadTexture(const char*)              -> void = 0;


  // Loads a file into a Font object and inserts it in map
  // by given name.
  virtual auto LoadFont(const char*, const char*, int) -> void = 0;


  // Unloads a Font object by it's name.
  virtual auto UnloadFont(const char*)                 -> void = 0;


  // Sets frame clear color.
  virtual auto SetClearColor(Uint8, Uint8, Uint8)      -> void = 0;


  // Sets current style draw color.
  virtual auto SetDrawColor(Uint8, Uint8, Uint8)       -> void = 0;


  // Sets current style font.
  virtual auto SetDrawFont(const char*)                -> void = 0;


  // Assigns frame width and hight in pixels to passed pointers.
  virtual auto GetResolution(int*, int*)               -> void = 0;


  // Returns the ratio of frame width to height.
  virtual auto GetAspectRatio()                        -> float = 0;


  // Returns a pointer to a Texture object by it's name.
  virtual auto GetTexture(const char*)                 -> Texture* = 0;


  // Draws a pixel at the given point in the frame. 
  // Coordinates are indicated in pixels.
  virtual auto DrawPoint(int, int)                     -> void = 0;


  // Draws a pixel at the given point in the frame.
  // Coordinates are normalised and indicated in value
  // between -1.0 and 1.0.
  virtual auto DrawPointN(float, float)                -> void = 0;


  // Draws a line between two given points in the frame.
  // Coordinates are indicated in pixels.
  virtual auto DrawLine(int, int, int, int)            -> void = 0;


  // Draws a line between two given points in the frame.
  // Coordinates are normalised and indicated in value
  // between -1.0 and 1.0.
  virtual auto DrawLineN(float, float, float, float)   -> void = 0;


  // Draws a triangle between the given three pixel points.
  // If fill is true, makes triangle filled with the current style
  // draw color. Coordinates are indicated in pixels.
  virtual auto DrawTriangle(
    int, int, 
    int, int,
    int, int,
    bool fill=true) -> void = 0;


  // Draws a triangle between the given three pixel points.
  // If fill is true, makes triangle filled with the current style
  // draw color. Coordinates are normalised and indicated in value
  // between -1.0 and 1.0.
  virtual auto DrawTriangleN(
    float, float, 
    float, float,
    float, float,
    bool fill=true) -> void = 0;


  // Draws a textured triangle between the given three pixel points. 
  // UV coordinates define normalized coordinates of the verticies
  // of the applied triangle texture fragment.
  // Frame coordinates are indicated in pixels.
  virtual auto DrawTriangleTextured(
    int, int,
    int, int,
    int, int,
    float, float,
    float, float,
    float, float,
    Texture*) -> void = 0;


  // Draws a textured triangle between the given three pixel points. 
  // UV coordinates define normalized coordinates of the verticies
  // of the applied triangle texture fragment. Frame coordinates are
  // normalised and indicated in value between -1.0 and 1.0.
  virtual auto DrawTriangleTexturedN(
    float, float,
    float, float,
    float, float,
    float, float,
    float, float,
    float, float,
    Texture*) -> void = 0;


  // Draws a rect between the given two pixel points. 
  // If fill is true, makes rect filled with the current style
  // draw color. Coordinates are indicated in pixels.
  virtual auto DrawRect(int, int, int, int, bool fill=true) -> void = 0;


  // Draws a rect between the given two pixel points. 
  // If fill is true, makes rect filled with the current style
  // draw color. Coordinates are normalised and indicated
  // in value between -1.0 and 1.0.
  virtual auto DrawRectN(float, float,
                         int, int, bool fill=true) -> void = 0;


  // Draws a Texture object specified by name at the given
  // point with the given scale. Coordinates are indicated in pixels.
  virtual auto DrawTexture(int, int, const char*, float, float) -> void = 0;


  // Draws a Texture object specified by name at the given
  // point with the given scale. Coordinates are normalised and indicated
  // in value between -1.0 and 1.0.
  virtual auto DrawTextureN(float, float,
                            const char*, float, float) -> void = 0;

  
  // Draws a Texture object specified by name at the given
  // point with the given scale rotated by specified angle.
  // Horizontal align can be set with 0 (left), 1 (center) or 2 (right).
  // Vertical align can be set with 0 (up), 1 (middle) or 2 (down).
  // Coordinates are indicated in pixels.
  virtual auto DrawTextureEx(
    int, int,
    const char*,
    float, float, float,
    int halign=0, int valign=0) -> void = 0;


  // Draws a Texture object specified by name at the given
  // point with the given scale rotated by specified angle.
  // Horizontal align can be set with 0 (left), 1 (center) or 2 (right).
  // Vertical align can be set with 0 (up), 1 (middle) or 2 (down).
  // Coordinates are normalised and indicated in value between -1.0 and 1.0.
  virtual auto DrawTextureExN(
    float, float,
    const char*,
    float, float, float,
    int halign=0, int valign=0) -> void = 0;


  // Renders text Texture object in the current style draw font
  // and draws it at the given point with the given scale.
  // Coordinates are indicated in pixels.
  virtual auto DrawText(int, int, const std::string&, 
                        float, float) -> void = 0;


  // Renders text Texture object in the current style draw font
  // and draws it at the given point with the given scale.
  // Coordinates are normalised and indicated in value between -1.0 and 1.0.
  virtual auto DrawTextN(float, float, const std::string&, 
                         float, float) -> void = 0;


  // Draw a rendered text Texture object at the given
  // point with the given scale rotated by specified angle.
  // Horizontal align can be set with 0 (left), 1 (center) or 2 (right).
  // Vertical align can be set with 0 (up), 1 (middle) or 2 (down).
  // Coordinates are indicated in pixels.
  virtual auto DrawTextEx(
    int, int,
    const std::string&,
    float, float, float,
    int halign=0, int valign=0) -> void = 0;


  // Draw a rendered text Texture object at the given
  // point with the given scale rotated by specified angle.
  // Horizontal align can be set with 0 (left), 1 (center) or 2 (right).
  // Vertical align can be set with 0 (up), 1 (middle) or 2 (down).
  // Coordinates are normalised and indicated in value between -1.0 and 1.0.
  virtual auto DrawTextExN(
    float, float,
    const std::string&,
    float, float, float,
    int halign=0, int valign=0) -> void = 0;


  // Returns Texture object that contains texture of rendered text.
  // If font wasn't specified the text will be rendered using current
  // style draw font.
  virtual auto RenderText(const std::string&,
                          const char* font="") -> Texture* = 0;

protected:
  // Constructor
  Drawer() = default;

  // Destructor
  virtual ~Drawer() = default;

};


}
