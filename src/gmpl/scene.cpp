#include "scene.h"
#include <stdexcept>

using namespace std;
using namespace engine::gfx;

namespace engine::gmpl {
  Scene::Scene(Window *p_window, Renderer *p_renderer, const float& p_deltaTime)
    : _window(p_window), _renderer(p_renderer), deltaTime(p_deltaTime) {}


  Scene::~Scene() {
    for(auto entity : _entitiesByType["all"]) { delete entity; }
    _entitiesByTag.clear();
    _entitiesByType.clear();
  }


  // - Methods -------------------------
  // ~ main
  auto Scene::update() -> void {
    // Update all entities states
    for (auto entity : _entitiesByType["all"]) { entity->on_update(); }

    // Draw all entities
    for (auto entity : _entitiesByType["all"]) { entity->on_draw(); }
  }


  // ~ camera
  auto Scene::get_camera() -> Camera* {
    return &_camera;
  }

  
  // ~ input
  auto Scene::input_check(SDL_Scancode keycode) -> bool {
    return _window->input_check(keycode);
  }

  auto Scene::input_axis(SDL_Scancode l, SDL_Scancode r) -> int {
    return _window->input_axis(l, r);
  }

  
  // ~ entities
  auto Scene::find_entity_by_type(const char *type, int n) -> Entity* {
    // Check if all entities of this type exist
    if (not _entitiesByType.contains(type))
      throw runtime_error("Entities of type \"" + string(type) + "\" weren't instantiated");

    // Check if index is valid
    if (_entitiesByType[type].size() <= n or n < 0)
      throw runtime_error("Wrong index");

    return _entitiesByType[type][n];
  }

  auto Scene::find_entity_by_tag(const char *tag) -> Entity* {
    // Check if entity with such tag exist
    if (not _entitiesByTag.contains(tag))
      throw runtime_error("No entity with tag \"" + string(tag) + "\"");

    return _entitiesByTag[tag];
  }

  // ~ draw
  auto Scene::set_draw_color(Uint8 r, Uint8 g, Uint8 b, Uint8 a) -> void {
    _window->set_draw_color(r, g, b, a);
  }

  auto Scene::set_font(const char* fontname) -> void {
    _window->set_font(fontname);
  }
  
  auto Scene::draw_line(int x1, int y1, int x2, int y2) -> void {
    _window->draw_line(x1, y1, x2, y2);
  }
  
  auto Scene::draw_text(const char* text, int x, int y, float xscale, float yscale) -> void {
    _window->draw_text(text, x, y, xscale, yscale);
  }

  auto Scene::draw_text_ex(const char* text, int x, int y, float xscale, float yscale, float angle) -> void {
    _window->draw_text_ex(text, x, y, xscale, yscale, angle);
  }

  auto Scene::draw_spirte(const char *spritename, int x, int y, float xscale, float yscale) -> void {
    _window->draw_sprite(spritename, x, y, xscale, yscale);
  }
  
  auto Scene::draw_spirte_ex(const char* spritename, int x, int y, float xscale, float yscale, float angle, int xcent, int ycent) -> void {
    _window->draw_sprite_ex(spritename, x, y, xscale, yscale, angle, xcent, ycent);
  }

  auto Scene::render_mesh(const char* meshname, math::vec3 pos, math::vec3 rot) -> void {
    _renderer->render_add_mesh(meshname, pos, rot);
  }
  
  // - Internal methods ----------------
  auto Scene::_initialize_entity(Entity *entity, const char *meshname, const char *type, const char *tag) -> void {
    entity->_scene = this;
    entity->_type = type+2;
    entity->_meshname = meshname;

    _entitiesByType["all"].push_back(entity);
    _entitiesByType[entity->_type].push_back(entity);

    if (tag[0] != '\0') { _entitiesByTag[tag] = entity; }

    entity->on_create();
  }
}
