#pragma once

#include <vector>

#include "gfx/renderer3d.h"
#include "gmpl/scene.h"
#include "gmpl/components/transform.h"

namespace eng::gmpl {

class Scene;
class Component;

class Entity {
friend Scene;
public:
  // - Methods -
  template<typename T>
  auto AddComponent() -> T* {
    static_assert(std::is_base_of<Component, T>(), "Given class not derrives from Component class");

    T* component = scene_->CreateComponent<T>(this);
    components_.push_back(component);
    return component;
  }

  Transform* const transform;

private:
  // Contructor
  Entity(Scene*, window::Window*, gfx::Renderer3D*, int);

  // Destructor
  ~Entity();

  std::vector<Component*> components_;

  Scene* const scene_;
  window::Window* const context_window_;
  gfx::Renderer3D* const renderer3d_;
  const int id_;
};

}
