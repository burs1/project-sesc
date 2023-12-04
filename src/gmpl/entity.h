#pragma once

#include "vec3.h"
#include <string>
#include "mesh.h"
#include "scene.h"
#include <stdexcept>

namespace engine::gmpl {
  class Scene;
  class Entity {
  friend class Scene;
  public:
    // - Variables -----------------------
    math::vec3 pos, rot;

  protected:
    // - Methods -------------------------
    auto get_delta_time() -> float;
    

    // ~ input
    auto input_check(SDL_Scancode) -> bool;

    auto input_axis(SDL_Scancode, SDL_Scancode) -> int;


    // ~ entities
    /*template<class T>
    auto instantiate_entity(int x = 0, int y = 0, std::string spriteName="") -> T* {
      return m_scene->instantiate_entity<T>(x, y, spriteName);
    }*/

    auto find_entity_by_type(const char*, int) -> Entity*;


    // ~ draw
    auto set_draw_color(Uint8, Uint8, Uint8, Uint8) -> void;

    auto set_font(const char*)            -> void;
    
    auto draw_line(int, int, int, int)    -> void;

    auto draw_text(const char*, int, int) -> void;

    auto draw_text_ex(const char*, int, int, float, float, float) -> void;

    auto draw_sprite(const char*, int, int, float xscale=1, float yscale=1) -> void;

    auto render_mesh(const char*, math::vec3, math::vec3) -> void;
    
    auto render_self() -> void;

  private:
    // - Events --------------------------
    virtual auto on_create()  -> void {}

    virtual auto on_update()  -> void {}

    virtual auto on_destroy() -> void {}

    virtual auto on_draw()    -> void { render_self(); }
    

    // - Variables -----------------------
    const char *_type;
    const char *_meshname = "";
    Scene *_scene;
  };
}
