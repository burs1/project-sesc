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
  virtual auto Present()                                       -> void override;


  virtual auto LoadTexture(const char *file, const char *name) -> void override;


  virtual auto UnloadTexture(const char*)                      -> void override;


  virtual auto LoadFont(const char *file, const char *name,
                        int size) -> void override;


  virtual auto UnloadFont(const char*)               -> void override;


  virtual auto SetClearColor(Uint8, Uint8, Uint8)    -> void override;


  virtual auto SetDrawColor(Uint8, Uint8, Uint8)     -> void override;


  virtual auto SetDrawFont(const char*)              -> void override;


  virtual auto GetResolution(int*, int*)             -> void override;


  virtual auto GetAspectRatio()                      -> float override;


  virtual auto GetTexture(const char*)               -> Texture* override;


  virtual auto DrawPoint(int, int)                   -> void override;


  virtual auto DrawPointN(float, float)              -> void override {}


  virtual auto DrawLine(int, int, int, int)          -> void override;


  virtual auto DrawLineN(float, float, float, float) -> void override {}


  virtual auto DrawTriangle(int, int, int, int, int, int,
                            bool fill=true) -> void override;


  virtual auto DrawTriangleN(float, float, float, float, float, float,
                             bool fill=true) -> void override {}


  virtual auto DrawTriangleTextured(
    int, int, int, int, int, int,
    float, float, float, float, float, float,
    Texture*) -> void override;


  virtual auto DrawTriangleTexturedN(
    float, float, float, float, float, float,
    float, float, float, float, float, float,
    Texture*) -> void override {}


  virtual auto DrawRect(int x1, int y1, int x2, int y2,
                        bool fill=true) -> void override;


  virtual auto DrawRectN(float x1, float y1, int x2, int y2,
                         bool fill=true) -> void override {}


  virtual auto DrawTexture(int x, int y, const char*,
                           float xscale, float yscale) -> void override;


  virtual auto DrawTextureN(float x, float y, const char*,
                            float xscale, float yscale) -> void override {}

  
  virtual auto DrawTextureEx(int x, int y, const char*,
                             float xscale, float yscale, float angle,
                             int halign=0, int valign=0) -> void override;


  virtual auto DrawTextureExN(float x, float y, const char*,
                              float xscale, float yscale, float angle,
                              int halign=0, int valign=0) -> void override {}


  virtual auto DrawText(int x, int y, const std::string&, 
                        float xscale, float yscale) -> void override;


  virtual auto DrawTextN(float x, float y, const std::string&, 
                         float xscale, float yscale) -> void override {}


  virtual auto DrawTextEx(int x, int y, const std::string&,
                          float xscale, float yscale, float angle,
                          int halign=0, int valign=0) -> void override;


  virtual auto DrawTextExN(float x, float y, const std::string&,
                           float xscale, float yscale, float angle,
                           int halign=0, int valign=0) -> void override {}

  virtual auto RenderText(const std::string&,
                          const char* font_name="") -> Texture* override;

private:
  DrawerSDL(SDL_Window*);

  ~DrawerSDL();

  // Uses SDL functionality to render text Texture object.
  auto RenderTextSDL(const char *text, const char *font_name,
                     SDL_Rect*) -> SDL_Texture*;

  SDL_Window* const sdl_window_;
  SDL_Renderer* sdl_renderer_;

  std::map<const char*, TTF_Font*> fonts_;
  std::map<const char*, Texture*> textures_;

  SDL_Color clear_color_ = {0, 0, 0, 255};
  SDL_Color draw_color_ = {255, 255, 255, 255};
  const char* draw_font_ = "";

};

}
