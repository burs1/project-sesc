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
  Scene(sdl::Window*, gfx::Renderer3D*, const float&);

  ~Scene();

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
  // Internal methods
  auto InitializeEntity(Entity*, const char*, const char*, const char*) -> void;

  // ~ vars
  sdl::Window *context_window_ = nullptr;
  gfx::Renderer3D *renderer3d_ = nullptr;

  std::map< const char*, Entity* >              entities_by_tag_;
  std::map< const char*, std::vector<Entity*> > entities_by_type_;

};

}
