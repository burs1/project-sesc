#include <string>
#include <stdexcept>

#include "gmpl/scene.h"
#include "gmpl/entity.h"
#include "gmpl/component.h"
#include "logger/logger.h"

namespace eng::gmpl {

// Constructor
Scene::Scene(window::Window* context_window, gfx::Renderer3D* renderer3d)
  : context_window_(context_window), renderer3d_(renderer3d),
    delta_time(delta_time_) {
  last_update_time_ = context_window_->GetTicks();
  log::Info("Scene created");
}


// Destructor
Scene::~Scene() {
  for (auto [id, entity] : entities_) {
    delete entity;
  }

  for (auto [id, component] : components_) {
    delete component;
  }

  log::Info("Scene destroyed");
}


// - Methods -
auto Scene::GetContextWindow() const -> window::Window* {
  return context_window_;
}


auto Scene::GetRenderer3D() const -> gfx::Renderer3D* {
  return renderer3d_;
}


auto Scene::UpdateComponents() -> void {
  for (auto component : dynamic_components_) {
    component->Update();
  }
}


auto Scene::CreateEntity() -> Entity* {
  int id = GetFreeEntityID();
  Entity* entity = new Entity(this, context_window_, renderer3d_, id);
  entities_[id] = entity;
  return entity;
}


auto Scene::DestroyEntity(Entity* entity)       -> void {
  for (auto component : entity->components_) {
    DestroyComponent(component);
  }
  entities_.erase(entity->id_);
  delete entity;
}


auto Scene::DestroyComponent(Component* component) -> void {
  components_.erase(component->id_);
  delete component;
}


auto Scene::GetFreeEntityID() -> int {
  for (int i = INT_MIN; i <= INT_MAX; ++i) {
    if (not entities_.contains(i)) { return i; }
  }
  throw std::runtime_error("No free entity id found");
}


auto Scene::GetFreeComponentID() -> int {
  for (int i = INT_MIN; i <= INT_MAX; ++i) {
    log::Info( std::to_string(components_.contains(i)) );
    // SEGFAULT HERE (at map::contains)
    if (not components_.contains(i)) { return i; }  }
  throw std::runtime_error("No free component id found");
}


}

