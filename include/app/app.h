#pragma once

#include "gmpl/scene.h"
#include "window/window.h"
#include "gfx/renderer3d.h"

namespace eng::app {

class App {
public:
  // Constructor. Loads resources from specified directory.
  App(const char*);

  // Destructor
  ~App();

  // - Methods -
  // This method calls before the initialisation of all engine systems.
  virtual auto OnAwake()   -> void {}


  // This methods calls after the initialisation of all engine systems.
  virtual auto OnCreate()  -> void {}


  // This methods call before window is goind to be closed and all engine
  // systems are deinitialised.
  virtual auto OnAsleep()  -> void {}

  // This methods calls after window closed and all engine
  // systems deinitialised.
  virtual auto OnDestroy() -> void {}


private:
  // - Internal methods -
  // Starts main game loop and stays in cycle until
  // window close is requested.
  auto MainLoop() -> void;

  // Load assets from specified directory.
  auto LoadAssets(const char*) -> void;

  // Vars
  window::Window* window_;
  gmpl::Scene* scene_;
  gfx::Renderer3D* renderer3d_;

  int update_delay_ = 16; // 16 ticks ~ 60 fps

};

}
