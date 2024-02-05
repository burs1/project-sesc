#pragma once

#include <memory>
#include <type_traits>

#include "window/window.h"
#include "gfx/renderer3d.h"
#include "gfx/components/mesh-renderer.h"
#include "gfx/components/texture-renderer.h"
#include "gmpl/component.h"

namespace eng::app {

class App; // forward declaration to make "App"
           // a friend class of Scene, so nobody else
           // couldn't instantiate it.

}

namespace eng::gmpl {

class Entity;
class Component;
class DynamicComponent;

class Scene {
friend app::App;
public:
  // - Readonly vars -
  const float &delta_time;

  // - Methods -
  // Updates all dynamic components in the scene.
  auto UpdateComponents()     -> void;


  // Creates an entity instance and returns a pointer to it.
  auto CreateEntity()         -> Entity*;


  // Destroys an entity instance.
  auto DestroyEntity(Entity*) -> void;


  // Creates a component instance and returns a pointer to it.
  template<typename T>
  auto CreateComponent(Entity *entity) -> T*
  {
    static_assert(std::is_base_of<Component, T>(), "Given class not derrives from Componrnt class");

    int id = GetFreeComponentID();
    T *component = new T();
    component->Init(entity, id);
    components_[id] = component;

    if (component->IsDynamic())
      dynamic_components_[id] = component;

    return component;
  }


  template<>
  auto CreateComponent<gfx::MeshRenderer>(
      Entity *entity) -> gfx::MeshRenderer*
  {
    int id = GetFreeComponentID();
    auto *component = new gfx::MeshRenderer();
    component->Init(entity, id);
    components_[id] = component;
    renderer_components_[id] = component;
    return component;
  }


  template<>
  auto CreateComponent<gfx::TextureRenderer>(
      Entity *entity) -> gfx::TextureRenderer*
  {
    int id = GetFreeComponentID();
    auto *component = new gfx::TextureRenderer();
    component->Init(entity, id);
    components_[id] = component;
    renderer_components_[id] = component;
    return component;
  }



  // Destroys a component instance.
  auto DestroyComponent(Component*) -> void;

private:
  Scene(window::Window* context_window,
        gfx::Renderer3D* renderer3d);

  ~Scene();

  // - Internal methods -
  // Returns the smallest free id for entity.
  auto GetFreeEntityID()    -> int;
  

  // Returns the smallest free id for component.
  auto GetFreeComponentID() -> int;


  float delta_time_ = 0.0f;

  window::Window *context_window_ = nullptr;
  gfx::Renderer3D *renderer3d_    = nullptr;

  std::map<int, Entity*>        entities_;
  std::map<int, Component*>     components_;
  std::map<int, gfx::Renderer*> renderer_components_;
  std::map<int, Component*>     dynamic_components_;

  Uint32 last_update_time_ = 0;

};

}
