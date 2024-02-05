#pragma once

namespace eng::gmpl {

class Scene;
class Entity;

class Component {
friend Scene;
protected:
  Component();

  ~Component();

  Entity*& entity;

private:
  // Sets a pointer to a parent entity and assigns this component's id.
  // Method moved from component constructor to avoid overriding constructor
  // for each derrived class.
  auto Init(Entity*, int) -> void;


  // Returns true if component must be updated each frame by scene.
  virtual auto IsDynamic() -> bool { return false; };


  // Calls each time "UpdateComponents" scene method called.
  auto Update() -> void { OnUpdate(); };


  // - Events -
  // Calls from "Init" method.
  virtual auto OnCreate()  -> void {};


  // Calls from "Update" method
  virtual auto OnUpdate()  -> void {};


  // Calls on component destruction.
  virtual auto OnDestroy() -> void {};


  Entity* entity_;
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
