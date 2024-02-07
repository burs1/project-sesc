#pragma once

#include "window/input.h"
#include "window/audio.h"
#include "window/drawer.h"

namespace eng::gmpl {

class Scene;
class Entity;

class Component {
friend Scene;
 protected:
  Component();
  ~Component();

  Scene  *&scene;
  Entity *&entity;

  window::Input  *&input;
  window::Audio  *&audio;
  window::Drawer *&drawer;

 private:
  // Sets a pointer to a parent entity and assigns this component's id.
  // Method moved from component constructor to avoid overriding constructor
  // for each derrived class.
  auto Init(Scene*, Entity*, int) -> void;

  // Returns true if component must be updated each frame by scene.
  virtual auto IsDynamic() -> bool { return false; };

  // Calls after "Init" method.
  virtual auto OnCreate()  -> void {};

  // Calls from "Update" method
  virtual auto OnUpdate()  -> void {};

  // Calls on component destruction.
  virtual auto OnDestroy() -> void {};

  Scene*  scene_;
  Entity* entity_;

  window::Input  *input_;
  window::Audio  *audio_;
  window::Drawer *drawer_;
  
  int id_;
};

class StaticComponent : public Component {
friend Scene;
 private:
  auto IsDynamic() -> bool override { return false; }
};

class DynamicComponent : public Component {
friend Scene;
 private:
  auto IsDynamic() -> bool override { return true; }
};

}

