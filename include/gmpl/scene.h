#pragma once

#include <type_traits>

#include "window/window.h"
#include "gfx/renderer3d.h"
#include "gmpl/component.h"
#include "logger/logger.h"

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
  // Returns a pointer to a Window object.
  auto GetContextWindow()     const -> window::Window*;


  // Returns a pointer to a Renderer3D object.
  auto GetRenderer3D()        const -> gfx::Renderer3D*;


  // Updates all dynamic components in the scene.
  auto UpdateComponents()           -> void;


  // Creates an entity instance and returns a pointer to it.
  auto CreateEntity()               -> Entity*;


  // Destroys an entity instance.
  auto DestroyEntity(Entity*)       -> void;


  // Creates a component instance and returns a pointer to it.
  template<typename T>
  auto CreateComponent(Entity* entity) -> T* {
    static_assert(std::is_base_of<Component, T>(), "Given class not derrives from Componrnt class");

    int id = GetFreeComponentID();
    T* component = new T();
    component->Init(entity, id);
    components_[id] = component;

    if (component->IsDynamic()) {
      dynamic_components_.push_back(component);
    }

    return component;
  }


  // Destroys a component instance.
  auto DestroyComponent(Component*) -> void;


private:
  // Constructor
  Scene(window::Window* context_window, gfx::Renderer3D* renderer3d);

  // Destructor
  ~Scene();

  // - Internal methods -
  // Returns the smallest free id for entity.
  auto GetFreeEntityID() -> int;
  

  // Returns the smallest free id for component.
  auto GetFreeComponentID() -> int;


  // Vars
  float delta_time_ = 0.0f;

  window::Window *context_window_ = nullptr;
  gfx::Renderer3D *renderer3d_ = nullptr;

  std::map<int, Entity*> entities_;

  std::map<int, Component*> components_;
  std::vector<Component*> dynamic_components_;

  Uint32 last_update_time_ = 0;

  static Scene* kInstance;

};

}
