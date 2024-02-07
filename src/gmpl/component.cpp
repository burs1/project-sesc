#include "gmpl/scene.h"
#include "gmpl/component.h"
#include "logger/logger.h"

namespace eng::gmpl {

Component::Component()
    : entity(entity_), scene(scene_),
      input(input_), audio(audio_), drawer(drawer_) {}


Component::~Component() {
  OnDestroy();
  log::Info("Component with id " + std::to_string(id_) + " destroyed");
}


auto Component::Init(Scene* scene, Entity* entity, int id) -> void
{
  scene_ = scene; entity_ = entity; id_ = id;
  auto context_window = scene->GetContextWindow();
  input_  = context_window->GetInput();
  audio_  = context_window->GetAudio();
  drawer_ = context_window->GetDrawer();
  OnCreate();
  log::Info("New component created with id " + std::to_string(id_));
}


}
