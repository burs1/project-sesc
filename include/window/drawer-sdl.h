#include <map>

#include <SDL.h>

#include "window/drawer.h"
#include "window/texture.h"

namespace eng::window {

class Window; // forward declaration to make "Window"
              // a friend class of DrawerSDL, so nobody else
              // couldn't instantiate it.

class DrawerSDL : public Drawer {
friend Window;
 public:
  auto Present()                                       -> void override;

  auto LoadTexture(std::string file, std::string name) -> void override;

  auto UnloadTexture(std::string)                      -> void override;

  auto LoadFont(std::string file, std::string name,
                        int size) -> void override;

  auto UnloadFont(std::string)               -> void override;

  auto SetClearColor(Uint8, Uint8, Uint8)    -> void override;

  auto SetDrawColor(Uint8, Uint8, Uint8)     -> void override;

  auto SetDrawFont(std::string)              -> void override;

  auto GetResolution(int*, int*)             -> void override;

  auto GetAspectRatio()                      -> float override;

  auto GetTexture(std::string)               -> const window::Texture* override;

  auto DrawPoint(int, int)                   -> void override;

  auto DrawPointN(float, float)              -> void override {}

  auto DrawLine(int, int, int, int)          -> void override;

  auto DrawLineN(float, float, float, float) -> void override {}

  auto DrawTriangle(int, int, int, int, int, int,
                    bool fill=true) -> void override;

  auto DrawTriangleN(float, float, float, float, float, float,
                     bool fill=true) -> void override {}

  auto DrawTriangleTextured(
    int, int, int, int, int, int,
    float, float, float, float, float, float,
    const Texture*) -> void override;

  auto DrawTriangleTexturedN(
    float, float, float, float, float, float,
    float, float, float, float, float, float,
    const Texture*) -> void override {}


  auto DrawRect(int x1, int y1, int x2, int y2,
                bool fill=true) -> void override;

  auto DrawRectN(float x1, float y1, int x2, int y2,
                 bool fill=true) -> void override {}

  auto DrawTexture(int x, int y, std::string,
                   float xscale, float yscale) -> void override;

  auto DrawTextureN(float x, float y, std::string,
                    float xscale, float yscale) -> void override {}

  auto DrawTextureEx(int x, int y, std::string,
                     float xscale, float yscale, float angle,
                     int halign=0, int valign=0) -> void override;

  auto DrawTextureExN(float x, float y, std::string,
                      float xscale, float yscale, float angle,
                      int halign=0, int valign=0) -> void override {}

  auto DrawText(int x, int y, const std::string&, 
                float xscale, float yscale) -> void override;

  auto DrawTextN(float x, float y, const std::string&, 
                 float xscale, float yscale) -> void override {}

  auto DrawTextEx(int x, int y, const std::string&,
                  float xscale, float yscale, float angle,
                  int halign=0, int valign=0) -> void override;

  auto DrawTextExN(float x, float y, const std::string&,
                   float xscale, float yscale, float angle,
                   int halign=0, int valign=0) -> void override {}

  auto RenderText(const std::string&,
                  const char* font_name="") -> window::Texture* override;


private:
  DrawerSDL(SDL_Window*);
  ~DrawerSDL();

  // Uses SDL functionality to render text Texture object.
  auto RenderTextSDL(std::string text, std::string font_name,
                     SDL_Rect*) -> SDL_Texture*;

  SDL_Window* const sdl_window_;
  SDL_Renderer* sdl_renderer_;

  std::map<std::string, TTF_Font*> fonts_;
  std::map<std::string, Texture*> textures_;

  SDL_Color clear_color_ = {0, 0, 0, 255};
  SDL_Color draw_color_ = {255, 255, 255, 255};
  std::string draw_font_ = "";
};

}

