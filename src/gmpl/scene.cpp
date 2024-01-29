#include "gmpl/scene.h"
#include "gmpl/entity.h"

namespace eng::gmpl {

Scene* Scene::kInstance = nullptr;

// Static methods
auto Scene::Create() -> void {
  if (not kInstance) {
    kInstance = new Scene();
    return;
  }
  throw std::runtime_error("Scene is already created.");
}


auto Scene::GetInstance() -> Scene* {
  if (kInstance) {
    return kInstance;
  }
  throw std::runtime_error("Scene doesn't exist.");
}


auto Scene::Destroy() -> void {
  if (kInstance) {
    delete kInstance;
    return;
  }
  throw std::runtime_error("Scene already doesn't exist.");
}


Scene::Scene()
  : context_window_(window::Window::GetInstance()),
    renderer3d_(gfx::Renderer3D::GetInstance()),
    delta_time(delta_time_) {
  last_update_time_ = context_window_->GetTicks();
}


Scene::~Scene() {
  for(auto entity : entities_by_type_["all"]) { delete entity; }
  entities_by_tag_.clear();
  entities_by_type_.clear();
}


// Methods
// ~ Main
auto Scene::Update() -> void {
  delta_time_ = (context_window_->GetTicks() - last_update_time_) / 1000.0f;
  last_update_time_ = context_window_->GetTicks();

  // Update all entities states
  for (auto entity : entities_by_type_["all"]) {
    entity->OnUpdate();
  }

  // Draw all entities
  for (auto entity : entities_by_type_["all"]) {
    entity->OnDraw();
  }

  renderer3d_->RenderQueue();

  // Draw GUI
  for (auto entity : entities_by_type_["all"]) {
    entity->OnDrawGUI();
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
auto Scene::InitializeEntity(Entity *entity, const char *mesh_name, const char *type, const char *tag) -> void {
  entity->mesh_name = mesh_name;

  entities_by_type_["all"].push_back(entity);
  entities_by_type_[type+2].push_back(entity);

  if (tag[0] != '\0') { entities_by_tag_[tag] = entity; }

  entity->OnCreate();
}


}
