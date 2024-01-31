#include <map>

#include "window/drawer.h"
#include "window/texture.h"

namespace eng::window {

class DrawerSDL : public Drawer {
friend Drawer;
public:
  // Methods
  // ~ Main
  auto Present()                               -> void override;

  // ~ Resources
  auto LoadTexture(const char*, const char*)   -> void override;

  auto UnloadTexture(const char*)              -> void override;

  auto LoadFont(const char*, const char*, int) -> void override;

  auto UnloadFont(const char*)                 -> void override;

  // ~ Setters
  auto SetClearColor(Uint8, Uint8, Uint8)      -> void override;

  auto SetDrawColor(Uint8, Uint8, Uint8)       -> void override;

  auto SetDrawFont(const char*)                -> void override;

  // ~ Getters
  auto GetResolution(int*, int*)               -> void override;

  auto GetAspectRatio()                        -> float override;

  auto GetTexture(const char*)                 -> Texture* override;

  // ~ Draw
  auto DrawPoint(int, int)                     -> void override;

  auto DrawLine(int, int, int, int)            -> void override;

  auto DrawTriangle(
    int, int, 
    int, int,
    int, int,
    bool fill=true) -> void override;

  auto DrawTriangleTextured(
    int, int,
    int, int,
    int, int,
    float, float,
    float, float,
    float, float,
    Texture*) -> void override;

  auto DrawRect(int, int, int, int, bool fill=true)     -> void override;

  auto DrawTexture(int, int, const char*, float, float) -> void override;
  
  auto DrawTextureEx(
    int, int,
    const char*,
    float, float, float,
    int halign=0, int valign=0) -> void override;

  auto DrawText(int, int, const std::string&,
      float, float) -> void override;

  auto DrawTextEx(
    int, int,
    const std::string&,
    float, float, float, 
    int halign=0, int valign=0) -> void override;

  // Rendering
  auto RenderText(const std::string&,
                  const char* font="") -> Texture* override;

private:
  DrawerSDL();

  ~DrawerSDL();

  // Internal methods
  // ~ Draw
  auto RenderTextSDL(const char*, const char*, SDL_Rect*) -> SDL_Texture*;

  // Vars
  SDL_Window* const sdl_window_;
  SDL_Renderer* sdl_renderer_;

  std::map<const char*, TTF_Font*> fonts_;
  std::map<const char*, Texture*> textures_;

  SDL_Color clear_color_ = {0, 0, 0, 255};
  SDL_Color draw_color_ = {255, 255, 255, 255};
  const char* draw_font_ = "";

};

}
