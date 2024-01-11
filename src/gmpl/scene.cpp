#include "gmpl/scene.h"
#include "gmpl/entity.h"

namespace eng::gmpl {

Scene::Scene(sdl::Window *context_window, gfx::Renderer3D *renderer3d,
             const float& delta_time)
  : context_window_(context_window), renderer3d_(renderer3d),
    delta_time(delta_time) {}


Scene::~Scene() {
  for(auto entity : entities_by_type_["all"]) { delete entity; }
  entities_by_tag_.clear();
  entities_by_type_.clear();
}


// Methods
// ~ Main
auto Scene::Update() -> void {
  // Update all entities states
  for (auto entity : entities_by_type_["all"]) {
    entity->OnUpdate();
  }

  // Draw all entities
  for (auto entity : entities_by_type_["all"]) {
    entity->OnDraw();
  }
}


// ~ entities
auto Scene::FindEntityByType(const char *type, int ind) -> Entity* {
  // Check if all entities of this type exist
  if (not entities_by_type_.contains(type)) {
    return nullptr;
  }

  // Check if index is valid
  if (entities_by_type_[type].size() <= ind or ind < 0) {
    return nullptr;
  }

  return entities_by_type_[type][ind];
}


auto Scene::FindEntityByTag(const char *tag) -> Entity* {
  // Check if entity with such tag exist
  if (not entities_by_tag_.contains(tag)) {
    return nullptr;
  }

  return entities_by_tag_[tag];
}


auto Scene::GetEntitiesCount(const char* type) -> int {
  return entities_by_type_[type].size();
}


// Internal methods
auto Scene::InitializeEntity(Entity *entity, const char *meshname, const char *type, const char *tag) -> void {
  entity->scene_ = this;
  entity->renderer3d_ = renderer3d_;
  entity->context_window_ = context_window_;
  entity->type_ = type+2;
  entity->meshname = meshname;

  entities_by_type_["all"].push_back(entity);
  entities_by_type_[entity->type_].push_back(entity);

  if (tag[0] != '\0') { entities_by_tag_[tag] = entity; }

  entity->OnCreate();
}

}
