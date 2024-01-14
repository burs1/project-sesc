#pragma once

#include <SDL_scancode.h>
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
  const char* meshname = "";

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


  // ~ Draw
  inline auto SetDrawColor(int r, int g, int b) -> void {
    drawer->SetDrawColor(r, g, b);
  }


  inline auto SetDrawFont(const char* name) -> void {
    drawer->SetDrawFont(name);
  }


  inline auto DrawLine(int x1, int y1, int x2, int y2) -> void {
    drawer->DrawLine(x1, y1, x2, y2);
  }

  
  inline auto DrawRect(int x1, int y1, int x2, int y2, bool fill=true) -> void {
    drawer->DrawRect(x1, y1, x2, y2, fill);
  }


  inline auto DrawTriangle(int x1, int y1, int x2, int y2, int x3, int y3,
                    bool fill=true) -> void {
    drawer->DrawTriangle(x1, y1, x2, y2, x3, y3, fill);
  }


  inline auto SetFont(const char* name) -> void {
    drawer->SetFont(name);
  }


  inline auto DrawText(int x, int y, const char* text,
                       float xscale=1, float yscale=1) -> void {
    drawer->DrawText(x, y, text, xscale,yscale);
  }

  inline auto DrawTextEx(int x, int y, const char* text,
                  float xscale, float yscale, float angle, 
                  int h_align=0, int v_align=0) -> void {
    drawer->DrawTextEx(x, y, text, xscale, yscale,
                                        angle, h_align, v_align);
  }
   
  inline auto DrawSprite(int x, int y, const char* name,
                         float xscale, float yscale) -> void {
    drawer->DrawSprite(x, y, name, xscale, yscale);
  }

  inline auto DrawSpriteEx(int x, int y, const char* name,
                    float xscale, float yscale, float angle,
                    int h_align=0, int v_align=0)   -> void {
    drawer->DrawSpriteEx(x, y, name, xscale, yscale,
                                          angle, h_align, v_align);
  }

  inline auto DrawMesh(const char* name, math::Vec3 pos, math::Vec3 rot, math::Vec3 scale) -> void {
    renderer3d->AddMeshToRenderHeap(name, pos, rot, scale);
  }

  inline auto DrawSelf() -> void {
    if (meshname[0] != '\0') {
      DrawMesh(meshname, pos, rot, scale);
    }
  }

  // Vars
  Scene*           const scene;
  sdl::Input*      const input;
  sdl::Drawer*     const drawer;
  gfx::Renderer3D* const renderer3d;

private:
  // Events
  virtual auto OnCreate()  -> void {}

  virtual auto OnUpdate()  -> void {}

  virtual auto OnDestroy() -> void {}

  virtual auto OnDraw()    -> void { DrawSelf(); }

  virtual auto OnGUIDraw() -> void {}
  
};

}
