#include <string>
#include <stdexcept>
#include <algorithm>

#include "gmpl/scene.h"
#include "gmpl/entity.h"
#include "gmpl/component.h"
#include "logger/logger.h"

namespace eng::gmpl {

Scene::Scene(window::Window* context_window,
             gfx::Renderer3D* renderer3d)
  : context_window_(context_window), renderer3d_(renderer3d),
    delta_time(delta_time_) {
  last_update_time_ = context_window_->GetTicks();
  log::Info("Scene created");
}


Scene::~Scene()
{
  for (auto [id, entity] : entities_)
    delete entity;

  for (auto [id, component] : components_)
    delete component;

  log::Info("Scene destroyed");
}


auto Scene::GetContextWindow() -> window::Window* {
  return context_window_;
}


auto Scene::UpdateComponents(float delta_time) -> void {
  delta_time_ = delta_time;
  for (auto [id, component] : dynamic_components_)
    component->OnUpdate();
}


auto Scene::CreateEntity() -> Entity*
{
  int id = GetFreeEntityID();
  Entity* entity = new Entity(this, context_window_, renderer3d_, id);
  entities_[id] = entity;
  return entity;
}


auto Scene::DestroyEntity(Entity* entity) -> void
{
  for (auto component : entity->components_)
    delete component;
  entities_.erase(entity->id_);
  delete entity;
  // Entity's components vector frees automaticly
}


auto Scene::InitComponent(Component* component, Entity* entity) -> int {
  int id = GetFreeComponentID();
  component->Init(this, entity, id);
  components_[id] = component;

  if (component->IsDynamic())
    dynamic_components_[id] = component;

  return id;
}

auto Scene::DestroyComponent(Component* component) -> void {
  // Erase component from it's entity components vector
  auto entity = component->entity_;
  auto component_allocator =
    std::find(
      entity->components_.begin(),
      entity->components_.end(),
      component);
  entity->components_.erase(component_allocator);

  // Erase component from vectors
  components_.erase(component->id_);
  dynamic_components_.erase(component->id_);
  renderer_components_.erase(component->id_);

  // Delete component object
  delete component;
}


auto Scene::GetFreeEntityID() -> int
{
  for (int i = INT_MIN; i <= INT_MAX; ++i)
    if (not entities_.contains(i)) { return i; }
  throw std::runtime_error("No free entity id found");
}


auto Scene::GetFreeComponentID() -> int
{
  for (int i = INT_MIN; i <= INT_MAX; ++i) 
    if (not components_.contains(i)) { return i; } 
  throw std::runtime_error("No free component id found");
}


}

