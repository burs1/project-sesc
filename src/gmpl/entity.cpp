#include "entity.h"

using namespace std;
using namespace engine::gfx;
using namespace engine::math;

namespace engine::gmpl {
  // - Methods -------------------------
  auto Entity::get_delta_time() -> float {
    return _scene->deltaTime;
  }

  // ~ input
  auto Entity::input_check(SDL_Scancode keycode) -> bool {
    return _scene->input_check(keycode);
  }

  auto Entity::input_axis(SDL_Scancode l, SDL_Scancode r) -> int {
    return _scene->input_axis(l, r);
  }


  // ~ entities
  auto Entity::find_entity_by_type(const char *type, int n) -> Entity* {
    return _scene->find_entity_by_type(type, n);
  }


  // ~ draw
  auto Entity::set_draw_color(Uint8 r, Uint8 g, Uint8 b, Uint8 a) -> void {
    _scene->set_draw_color(r, g, b, a);
  }

  auto Entity::set_font(const char *fontname) -> void {
    _scene->set_font(fontname);
  }
  
  auto Entity::draw_line(int x1, int y1, int x2, int y2) -> void {
    _scene->draw_line(x1, y1, x2, y2);
  }

  auto Entity::draw_text(const char *text, int x, int y) -> void {
    _scene->draw_text(text, x, y, 1, 1);
  }
  
  auto Entity::draw_text_ex(const char *text, int x, int y, float xscale, float yscale, float angle) -> void {
    _scene->draw_text_ex(text, x, y, xscale, yscale, angle);
  }

  auto Entity::draw_sprite(const char *spritename, int x, int y, float xscale, float yscale) -> void {
    _scene->draw_spirte(spritename, x, y, xscale, yscale);
  }

  auto Entity::render_mesh(const char* meshname, vec3 pos, vec3 rot) -> void {
    _scene->render_mesh(meshname, pos, rot);
  }

  auto Entity::render_self() -> void {
    if (_meshname[0] != '\0') { _scene->render_mesh(_meshname, pos, rot); }
  }
}
