#pragma once

#include "gmpl/scene.h"
#include "window/window.h"
#include "gfx/renderer3d.h"

namespace eng::app {

class App {
 public:
  App(const char*);
  ~App();

  // This method calls before the initialisation of all engine systems.
  virtual auto OnAwake()   -> void {}

  // This methods calls after the initialisation of all engine systems.
  virtual auto OnCreate()  -> void {}

  // Calls before window is goind to be closed and all engine
  // systems are deinitialised.
  virtual auto OnFall()  -> void {}

  // Calls after window closed and all engine
  // systems deinitialised.
  virtual auto OnDestroy() -> void {}

private:
  // Starts main game loop and stays in cycle until
  // window close is requested.
  auto MainLoop() -> void;

  // Load assets from specified directory.
  auto LoadAssets(const char*) -> void;

  gmpl::Scene* scene_;
  window::Window* window_;
  gfx::Renderer3D* renderer3d_;

  int update_delay_ = 16; // 16 ticks ~ 60 fps

};

}
