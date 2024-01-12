#pragma once

#include <map>
#include <string>
#include <stdexcept>

#include <SDL.h>
#include <SDL_ttf.h>
#include <SDl_image.h>

#include "window/sprite.h"

namespace eng::sdl {

class Drawer {
public:
  Drawer(SDL_Window*);

  ~Drawer();

  // Not copyable
  Drawer(const Drawer&) = delete;

  Drawer operator=(const Drawer&) = delete;

  // Not movable
  Drawer(Drawer&&) = delete;

  Drawer& operator=(Drawer&&) = delete;

  // Methods
  // ~ Main
  auto Present()                                    -> void;

  // ~ Resources
  auto LoadSprite(const char*, const char*)         -> void;

  auto UnloadSprite(const char*)                    -> void;

  auto LoadFont(const char*, const char*, int)      -> void;

  auto UnloadFont(const char*)                      -> void;

  // ~ Draw
  auto SetRenderLogicalSize(int w, int h)           -> void;

  auto SetClearColor(Uint8, Uint8, Uint8)           -> void;

  auto SetDrawColor(Uint8, Uint8, Uint8)            -> void;

  auto SetDrawFont(const char*)                     -> void;

  auto DrawLine(int, int, int, int)                 -> void;

  auto DrawRect(int, int, int, int, bool fill=true) -> void;

  auto DrawTriangle(int, int, int, int, int, int,
                    bool fill=true) -> void;

  auto DrawTriangleTextured(int, int, int, int, int, int,
                            float, float, float, float, float, float,
                            const char*) -> void;

  auto SetFont(const char*) -> void;

  auto DrawText(int, int, const char*, float, float) -> void;

  auto DrawTextEx(int, int, const char*, float, float, float, 
                  int h_align=0, int v_align=0) -> void;
   
  auto DrawSprite(int, int, const char*, float, float) -> void;
  
  auto DrawSpriteEx(int, int, const char*, float, float, float,
                    int h_align=0, int v_align=0)   -> void;

  // Readonly vars
  const int& resw;
  const int& resh;

private:
  // Internal methods
  // ~ Draw
  auto RenderText(const char*, SDL_Rect*) -> SDL_Texture*;

  // Vars
  SDL_Renderer* sdl_renderer_ = nullptr;

  std::map<const char*, TTF_Font*> fonts_;
  std::map< const char*, Sprite* > sprites_;
  const char* draw_font_ = "";

  SDL_Color clear_color_ = {0, 0, 0, 255};
  SDL_Color draw_color_ = {255, 255, 255, 255};

  int resw_ = 0.0f;
  int resh_ = 0.0f;
};

}
