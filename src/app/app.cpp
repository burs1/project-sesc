#include <exception>
#include <stdexcept>
#include <string>
#include <fstream>
#include <strstream>

#include "app/app.h"
#include "gfx/components/mesh-renderer.h"
#include "gmpl/scene.h"
#include "gmpl/entity.h"
#include "gmpl/components/utils.h"
#include "math/vec3.h"
#include "window/window.h"
#include "gfx/renderer3d.h"
#include "logger/logger.h"

namespace eng::app {

App::App(std::string wad_file) {
  OnAwake();

  // Initialise main engine systems
  log::Info("Initalising engine systems");

  window_ = new window::Window("", 1280, 720);
  renderer3d_ = new gfx::Renderer3D(window_->GetDrawer());
  scene_ = new gmpl::Scene(window_, renderer3d_);
  renderer3d_->SetRendererComponentsMap(&scene_->renderer_components_);

  log::Info("Engine systems initialised successfuly");

  OnCreate();
  LoadAssets(wad_file);

  // ------- DEBUG -----------
  auto e = scene_->CreateEntity();
  auto cmp = e->AddComponent<gfx::MeshRenderer>();
  e->AddComponent<gmpl::Rotator>();
  cmp->SetMesh( renderer3d_->GetMesh("cube") );
  e->transform->SetPosition(math::Vec3(0.0f, 0.0f, 10.0f));
  e->transform->SetRotation(math::Vec3(0.0f, 0.0f, 0.0f));

  window_->GetDrawer()->SetClearColor(15, 15, 15);
  renderer3d_->SetSunRotation(math::Vec3(3.0f, 0.0f, 0.0f));
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
    Uint32 current_time = window_->GetTicks();
    if (current_time < next_update_time) { continue; }

    float delta_time = (current_time - last_update_time) / 1000.0f;
    last_update_time = current_time;
    next_update_time = current_time - (current_time % update_delay_) + update_delay_;

    window_->PollEvents();
    scene_->UpdateComponents(delta_time);
    renderer3d_->RenderFrame();
    window_->UpdateSurface();
  }

  log::Info("Exiting main game loop");
}


auto App::LoadAssets(std::string wad_file) -> void {
  log::Info("Loading assets using \"" + wad_file + "\" WAD file");

  std::ifstream in(wad_file);

  // Check if file successfuly opened
  if (not in.is_open())
    throw std::runtime_error("File \"" + wad_file + "\" couldn't be opened");

  // Read file
  std::strstream strstream;
  std::string file, name;
  char type, line[256];
  int font_size;

  while (!in.eof()) {
    in.getline(line, 256);
    if (line[0] == '\0') { break; }

    strstream << line;
    strstream >> type >> file >> name;
    puts(name.c_str());
    switch (type) {
      case 'f': {
        strstream >> font_size;
        window_->GetDrawer()->LoadFont(
            "assets/fonts/"+file, name, font_size);
        break;
      }
      case 'm': {
        renderer3d_->LoadMesh(
            "assets/models/"+file, name);
        break;
      }
      case 't': {
        window_->GetDrawer()->LoadTexture(
            "assets/textures/"+file, name);
        break;
      }
      case 's': {
        window_->GetAudio()->LoadSound(
            ("assets/sounds/"+file), name);
      }
    }
  }

  log::Info("Assets loaded successfuly");
}


}
