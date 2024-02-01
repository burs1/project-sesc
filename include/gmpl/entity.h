#pragma once

#include <string>
#include <stdexcept>

#include "gfx/mesh.h"
#include "gfx/renderer3d.h"
#include "math/vec3.h"
#include "gmpl/scene.h"

namespace eng::gmpl {

class Entity {
friend Scene;
public:
  Entity();

  // Vars
  math::Vec3 pos, rot;
  math::Vec3 scale = math::Vec3(1.0f, 1.0f, 1.0f);
  const char* mesh_name = "";

protected:
  // Methods
  // Main
  inline auto GetDeltaTime() -> float {
    return scene->delta_time;
  }

  // ~ Keyboard input
  inline auto IsKeyDown(SDL_Scancode key) const -> bool {
    return input->IsKeyDown(key);
  }


  inline auto IsKeyPressed(SDL_Scancode key) const -> bool {
    return input->IsKeyPressed(key);
  }


  inline auto IsKeyReleased(SDL_Scancode key) const -> bool {
    return input->IsKeyReleased(key);
  }


  // ~ Mouse input
  inline auto IsMouseButtonDown(int button) const -> bool {
    return input->IsMouseButtonDown(button);
  }


  inline auto IsMouseButtonPressed(int button) const -> bool {
    return input->IsMouseButtonPressed(button);
  }


  inline auto IsMouseButtonReleased(int button) const -> bool {
    return input->IsMouseButtonReleased(button);
  }

  
  inline auto GetMousePosition(int* x, int* y) const -> void {
    input->GetMousePosition(x, y);
  }


  inline auto GetMouseDelta(int* x, int* y) const -> void {
    input->GetMouseDelta(x, y);
  }


  // ~ Camera
  inline auto SetCameraTransform(
      const math::Vec3* pos,
      const math::Vec3* rot) -> void {
    renderer3d->SetCameraTransform(pos, rot);
  }

  inline auto SetCameraFOV(float fov) -> void {
    renderer3d->SetPerspective(fov, 0.1f, 1000.0f);
  }


  // ~ Entities
  inline auto FindEntityByType(const char* type, int ind) -> Entity* {
    return scene->FindEntityByType(type, ind);
  }


  inline auto FindEntityByTag(const char* tag) -> Entity* {
    return scene->FindEntityByTag(tag);
  }


  // ~ Draw settings
  inline auto SetDrawColor(int r, int g, int b) -> void {
    drawer->SetDrawColor(r, g, b);
  }


  inline auto SetDrawFont(const char* name) -> void {
    drawer->SetDrawFont(name);
  }


  // ~ Draw
  inline auto DrawLine(int x1, int y1, int x2, int y2) -> void {
    drawer->DrawLine(x1, y1, x2, y2);
  }

  
  inline auto DrawRect(int x1, int y1, int x2, int y2, bool fill=true) -> void {
    drawer->DrawRect(x1, y1, x2, y2, fill);
  }


  inline auto DrawTriangle(
      int x1, int y1,
      int x2, int y2,
      int x3, int y3,
      bool fill=true) -> void {
    drawer->DrawTriangle(x1, y1, x2, y2, x3, y3, fill);
  }


  inline auto DrawText(
      int x, int y,
      const std::string& text,
      float xscale=1, float yscale=1) -> void {
    drawer->DrawText(x, y, text, xscale,yscale);
  }


  inline auto DrawTextEx(
      int x, int y,
      const std::string& text,
      float xscale, float yscale, float angle, 
      int h_align=0, int v_align=0) -> void {
    drawer->DrawTextEx(
      x, y,
      text,
      xscale, yscale, angle,
      h_align, v_align);
  }
  

  inline auto DrawTexture(
      int x, int y,
      const char* texture_name,
      float xscale, float yscale) -> void {
    drawer->DrawTexture(x, y, texture_name, xscale, yscale);
  }


  inline auto DrawTextureEx(
      int x, int y,
      const char* texture_name,
      float xscale, float yscale, float angle,
      int h_align=0, int v_align=0) -> void {
    drawer->DrawTextureEx(
      x, y,
      texture_name,
      xscale, yscale, angle,
      h_align, v_align);
  }


  // ~ Draw 3D
  inline auto DrawMesh(
      const math::Vec3& pos,
      const math::Vec3& rot,
      const math::Vec3& scale,
      const char* mesh_name,
      bool ignore_lightning=false) -> void {
    renderer3d->AddMeshToQueue(pos, rot, scale, ignore_lightning, mesh_name);
  }


  inline auto DrawTexture3D(
      const math::Vec3& pos,
      const math::Vec3& rot,
      const math::Vec3& scale,
      const char* texture_name,
      bool ignore_lightning=true) -> void {
    renderer3d->AddTextureToQueue(pos, rot, scale, ignore_lightning, texture_name);
  }

  
  inline auto DrawText3D(
      const math::Vec3& pos,
      const math::Vec3& rot,
      const math::Vec3& scale,
      const char* text,
      bool ignore_lightning=true) -> void {
    renderer3d->AddTextToQueue(pos, rot, scale, ignore_lightning, text);
  }


  inline auto DrawSelf() -> void {
    if (mesh_name[0] != '\0') {
      DrawMesh(pos, rot, scale, mesh_name);
    }
  }


  // Vars
  Scene*           const scene;
  window::Input*      const input;
  window::Drawer*     const drawer;
  gfx::Renderer3D* const renderer3d;

private:
  // Events
  virtual auto OnCreate()  -> void {}

  virtual auto OnUpdate()  -> void {}

  virtual auto OnDestroy() -> void {}

  virtual auto OnDraw()    -> void { DrawSelf(); }

  virtual auto OnDrawGUI() -> void {}
  
};

}
