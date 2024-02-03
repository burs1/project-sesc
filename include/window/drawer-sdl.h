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
  virtual auto Present()                               -> void override;


  virtual auto LoadTexture(const char*, const char*)   -> void override;


  virtual auto UnloadTexture(const char*)              -> void override;


  virtual auto LoadFont(const char*, const char*, int) -> void override;


  virtual auto UnloadFont(const char*)                 -> void override;


  virtual auto SetClearColor(Uint8, Uint8, Uint8)      -> void override;


  virtual auto SetDrawColor(Uint8, Uint8, Uint8)       -> void override;


  virtual auto SetDrawFont(const char*)                -> void override;


  virtual auto GetResolution(int*, int*)               -> void override;


  virtual auto GetAspectRatio()                        -> float override;


  virtual auto GetTexture(const char*)                 -> Texture* override;


  virtual auto DrawPoint(int, int)                     -> void override;


  virtual auto DrawPointN(float, float)                -> void override {}


  virtual auto DrawLine(int, int, int, int)            -> void override;


  virtual auto DrawLineN(float, float, float, float)   -> void override {}


  virtual auto DrawTriangle(
    int, int, 
    int, int,
    int, int,
    bool fill=true) -> void override;


  virtual auto DrawTriangleN(
    float, float, 
    float, float,
    float, float,
    bool fill=true) -> void override {}


  virtual auto DrawTriangleTextured(
    int, int,
    int, int,
    int, int,
    float, float,
    float, float,
    float, float,
    Texture*) -> void override;


  virtual auto DrawTriangleTexturedN(
    float, float,
    float, float,
    float, float,
    float, float,
    float, float,
    float, float,
    Texture*) -> void override {}


  virtual auto DrawRect(int, int, int, int, bool fill=true) -> void override;


  virtual auto DrawRectN(float, float,
                         int, int, bool fill=true) -> void override {}


  virtual auto DrawTexture(int, int, const char*, float, float) -> void override;


  virtual auto DrawTextureN(float, float,
                            const char*, float, float) -> void override {}

  
  virtual auto DrawTextureEx(
    int, int,
    const char*,
    float, float, float,
    int halign=0, int valign=0) -> void override;


  virtual auto DrawTextureExN(
    float, float,
    const char*,
    float, float, float,
    int halign=0, int valign=0) -> void override {}


  virtual auto DrawText(int, int, const std::string&, 
                        float, float) -> void override;


  virtual auto DrawTextN(float, float, const std::string&, 
                         float, float) -> void override {}


  virtual auto DrawTextEx(
    int, int,
    const std::string&,
    float, float, float,
    int halign=0, int valign=0) -> void override;


  virtual auto DrawTextExN(
    float, float,
    const std::string&,
    float, float, float,
    int halign=0, int valign=0) -> void override {}


  virtual auto RenderText(const std::string&,
                          const char* font="") -> Texture* override;

private:
  // Constructor
  DrawerSDL(SDL_Window*);

  // Destructor
  ~DrawerSDL();

  // - Initial methods -
  // Uses SDL functionality to render text Texture object.
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
