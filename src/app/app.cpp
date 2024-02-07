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
  auto OnCreate() -> void override {
    entity->transform->SetRotation( eng::math::Vec3(0.1f, 0.0f, 0.1f) );
    SetCursorLock(true);
  }

  auto OnUpdate() -> void override {
    int dx = 0, dy;
    GetMouseDelta(&dx, &dy);

    if (IsMouseButtonPressed(1))
      SetCursorLock(false);

    entity->transform->Rotate( eng::math::Vec3(0.0f, -dx * 0.005f, 0.0f) );
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
  auto cmp = e->AddComponent<gfx::MeshRenderer>();
  cmp->SetMesh( renderer3d_->meshes_["pc"] );
  cmp->SetTexture( window_->drawer_->GetTexture("pc") );
  e->transform->SetPosition(math::Vec3(0.0f, 0.0f, 4.0f));
  e->AddComponent<Rotator>();

  window_->drawer_->SetClearColor(212, 156, 25);

  renderer3d_->SetSunRotation(math::Vec3(3.0f, 0.0f, 0.0f));
  renderer3d_->SetSunColor(212, 156, 25);
  // -------------------------

  MainLoop();
}

App::~App() {
  OnFall();

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
  renderer3d_->LoadMesh("assets/models/pc.obj", "pc");
  window_->drawer_->LoadTexture("assets/sprites/pc.png", "pc");

  log::Info("Assets loaded successfuly");
}


}
