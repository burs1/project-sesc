#pragma once

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
  // Returns a pointer to a context window.
  auto GetContextWindow()      -> window::Window*;

  // Updates all dynamic components in the scene.
  auto UpdateComponents(float) -> void;

  // Creates an entity instance and returns a pointer to it.
  auto CreateEntity()          -> Entity*;

  // Destroys an entity instance.
  auto DestroyEntity(Entity*)  -> void;

  // Creates a component instance and returns a pointer to it.
  template<typename T>
  auto CreateComponent(Entity *entity) -> T*
  {
    static_assert(std::is_base_of<Component, T>(), "Given class not derrives from Componrnt class");
    T *component = new T();
    InitComponent(component, entity);
    return component;
  }

  template<>
  auto CreateComponent<gfx::MeshRenderer>(
      Entity *entity) -> gfx::MeshRenderer*
  {
    auto *component = new gfx::MeshRenderer();
    int id = InitComponent(component, entity);
    renderer_components_[id] = component;
    return component;
  }

  template<>
  auto CreateComponent<gfx::TextureRenderer>(
      Entity *entity) -> gfx::TextureRenderer*
  {
    auto *component = new gfx::TextureRenderer();
    int id = InitComponent(component, entity);
    renderer_components_[id] = component;
    return component;
  }

  // Destroys a component instance.
  auto DestroyComponent(Component*) -> void;
  
  const float &delta_time;

 private:
  Scene(window::Window* context_window, gfx::Renderer3D* renderer3d);
  ~Scene();
  
  // Calls "Init" and "OnCreate" methods of passed component and inserts
  // in "dynamic_components_" if it's dynamic. Returns component's id.
  auto InitComponent(Component*, Entity*) -> int;

  // Returns the smallest free id for entity.
  auto GetFreeEntityID()         -> int;

  // Returns the smallest free id for component.
  auto GetFreeComponentID()      -> int;

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

