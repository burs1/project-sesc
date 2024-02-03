#include "gmpl/scene.h"
#include "gmpl/entity.h"
#include "gmpl/components/transform.h"
#include "logger/logger.h"

namespace eng::gmpl {
  Entity::Entity(Scene* scene, window::Window* context_window,
                 gfx::Renderer3D* renderer3d, int id)
    : scene_(scene), context_window_(context_window),
      renderer3d_(renderer3d), id_(id), transform(AddComponent<Transform>()) {
    log::Info("New entity created with id " + std::to_string(id_));
  }


  Entity::~Entity() {
    log::Info("Entity with id " + std::to_string(id_) + " destroyed");
  }
}
