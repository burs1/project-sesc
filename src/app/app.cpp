#include <string>

#include "app/app.h"
#include "gfx/components/mesh-renderer.h"
#include "gmpl/scene.h"
#include "gmpl/entity.h"
#include "gmpl/components/behaviour.h"
#include "math/vec3.h"
#include "window/window.h"
#include "gfx/renderer3d.h"
#include "logger/logger.h"

class Rotator : public eng::gmpl::Behaviour {
private:
  auto OnUpdate() -> void override {
    entity->transform->Rotate( eng::math::Vec3(0.0f, 0.01f, 0.0f) );
    entity->transform->Translate( eng::math::Vec3(0.0f, 0.0f, -0.1f) );
  }
};

namespace eng::app {

App::App(const char* assets_dir) {
  OnAwake();

  // Initialise main engine systems
  log::Info("Initalising engine systems");

  window_ = new window::Window("", 1280, 720);
  renderer3d_ = new gfx::Renderer3D(window_->GetDrawer());
  scene_ = new gmpl::Scene(window_, renderer3d_);

  renderer3d_->SetRendererComponentsMap(&scene_->renderer_components_);

  log::Info("Engine systems initialised successfuly");

  OnCreate();

  LoadAssets(assets_dir);

  // ------- DEBUG -----------
  auto e = scene_->CreateEntity();
  e->AddComponent<gfx::MeshRenderer>()->SetMesh( renderer3d_->meshes_["cube"] );
  e->transform->SetPosition(math::Vec3(0.0f, 0.0f, 100.0f));
  e->AddComponent<Rotator>();

  window_->drawer_->SetClearColor(25, 25, 25);
  // -------------------------

  MainLoop();
}

App::~App() {
  OnAsleep();

  // Deinitialise main engine systems
  delete scene_;
  delete renderer3d_;
  delete window_;

  OnDestroy();
}

// - Internal methods -
auto App::MainLoop() -> void {
  log::Info("Entering main game loop");

  // Refresh rate controll
  Uint32 last_update_time = window_->GetTicks();
  Uint32 next_update_time = last_update_time;

  // Main game loop
  while (not window_->IsCloseRequested()) {
    // Hold update untill it's time
    Uint32 currentTime = window_->GetTicks();
    if (currentTime < next_update_time) { continue; }

    last_update_time = currentTime;
    next_update_time = currentTime - (currentTime % update_delay_) + update_delay_;

    window_->PollEvents();
    scene_->UpdateComponents();
    renderer3d_->RenderFrame();
    window_->UpdateSurface();
  }

  log::Info("Exiting main game loop");
}


auto App::LoadAssets(const char* assets_dir) -> void {
  log::Info("Loading assets from \"" + std::string(assets_dir) + "\"");

  // do some cool stuff...
  renderer3d_->LoadMesh("assets/models/cube.obj", "cube");

  log::Info("Assets loaded successfuly");
}


}
