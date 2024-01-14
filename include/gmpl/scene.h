#pragma once

#include <map>
#include <vector>
#include <string>

#include "window/window.h"
#include "gfx/renderer3d.h"

namespace eng::gmpl {

class Entity;
class Scene {
public:
  // Static methods
  static auto Init()        -> void;

  static auto GetInstance() -> Scene*;

  static auto Quit()        -> void;

  // Methods
  // ~ Main
  auto Update() -> void;

  template<class T>
  auto InstantiateEntity(const char* meshname="\0", const char* tag="\0") -> T* {
    static_assert(std::is_base_of<Entity, T>(), "This class doesn't derrives from the Entity class");

    T *entity = new T();
    InitializeEntity(entity, meshname, typeid(entity).name(), tag);
    return entity;
  }

  // ~ Entities
  auto FindEntityByType(const char*, int) -> Entity*;

  auto FindEntityByTag(const char*)       -> Entity*;

  auto GetEntitiesCount(const char*)      -> int;

  // Vars
  const float &delta_time;

private:
  Scene();

  ~Scene();

  // Internal methods
  auto InitializeEntity(Entity*, const char*, const char*, const char*) -> void;

  // Vars
  float delta_time_ = 0.0f;

  sdl::Window *context_window_ = nullptr;
  gfx::Renderer3D *renderer3d_ = nullptr;

  std::map< const char*, Entity* >              entities_by_tag_;
  std::map< const char*, std::vector<Entity*> > entities_by_type_;

  Uint32 last_update_time_ = 0;

  static Scene* kInstance;

};

}
