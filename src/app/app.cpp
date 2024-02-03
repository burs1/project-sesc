#include <string>

#include "app/app.h"
#include "gmpl/scene.h"
#include "gmpl/entity.h"
#include "gmpl/components/behaviour.h"
#include "window/window.h"
#include "gfx/renderer3d.h"
#include "logger/logger.h"

namespace eng::app {

App::App(const char* assets_dir) {
  OnAwake();

  // Initialise main engine systems
  log::Info("Initalising engine systems");

  window_ = new window::Window("", 1280, 720);
  renderer3d_ = new gfx::Renderer3D(window_->GetDrawer());
  scene_ = new gmpl::Scene(window_, renderer3d_);

  log::Info("Engine systems initialised successfuly");

  OnCreate();

  LoadAssets(assets_dir);

  // ------- DEBUG -----------
  scene_->CreateEntity();
  scene_->CreateEntity();
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
    window_->UpdateSurface();
  }

  log::Info("Exiting main game loop");
}


auto App::LoadAssets(const char* assets_dir) -> void {
  log::Info("Loading assets from \"" + std::string(assets_dir) + "\"");

  // do some cool stuff...

  log::Info("Assets loaded successfuly");
}


}
