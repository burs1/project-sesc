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
friend class Scene;
public:
  // Vars
  math::Vec3 pos, rot;
  math::Vec3 scale = math::Vec3(1.0f, 1.0f, 1.0f);
  const char* meshname = "";

protected:
  // Methods
  // Main
  inline auto GetDeltaTime() -> float {
    return scene_->delta_time;
  }

  // ~ Keyboard input
  inline auto IsKeyDown(SDL_Scancode key) const -> bool {
    return context_window_->input->IsKeyDown(key);
  }


  inline auto IsKeyPressed(SDL_Scancode key) const -> bool {
    return context_window_->input->IsKeyPressed(key);
  }


  inline auto IsKeyReleased(SDL_Scancode key) const -> bool {
    return context_window_->input->IsKeyReleased(key);
  }


  // ~ Mouse input
  inline auto IsMouseButtonDown(int button) const -> bool {
    return context_window_->input->IsMouseButtonDown(button);
  }


  inline auto IsMouseButtonPressed(int button) const -> bool {
    return context_window_->input->IsMouseButtonPressed(button);
  }


  inline auto IsMouseButtonReleased(int button) const -> bool {
    return context_window_->input->IsMouseButtonReleased(button);
  }

  
  inline auto GetMousePosition(int* x, int* y) const -> void {
    context_window_->input->GetMousePosition(x, y);
  }


  inline auto GetMouseDelta(int* x, int* y) const -> void {
    context_window_->input->GetMouseDelta(x, y);
  }


  // ~ Camera
  inline auto SetCameraTransform(
      const math::Vec3& pos,
      const math::Vec3& rot) -> void {
    renderer3d_->SetCameraTransform(pos, rot);
  }


  // ~ Entities
  inline auto FindEntityByType(const char* type, int ind) -> Entity* {
    return scene_->FindEntityByType(type, ind);
  }


  inline auto FindEntityByTag(const char* tag) -> Entity* {
    return scene_->FindEntityByTag(tag);
  }


  // ~ Draw
  inline auto SetDrawColor(int r, int g, int b) -> void {
    context_window_->drawer->SetDrawColor(r, g, b);
  }


  inline auto SetDrawFont(const char* name) -> void {
    context_window_->drawer->SetDrawFont(name);
  }


  inline auto DrawLine(int x1, int y1, int x2, int y2) -> void {
    context_window_->drawer->DrawLine(x1, y1, x2, y2);
  }

  
  inline auto DrawRect(int x1, int y1, int x2, int y2, bool fill=true) -> void {
    context_window_->drawer->DrawRect(x1, y1, x2, y2, fill);
  }


  inline auto DrawTriangle(int x1, int y1, int x2, int y2, int x3, int y3,
                    bool fill=true) -> void {
    context_window_->drawer->DrawTriangle(x1, y1, x2, y2, x3, y3, fill);
  }


  inline auto SetFont(const char* name) -> void {
    context_window_->drawer->SetFont(name);
  }


  inline auto DrawText(int x, int y, const char* text,
                       float xscale=1, float yscale=1) -> void {
    context_window_->drawer->DrawText(x, y, text, xscale,yscale);
  }

  inline auto DrawTextEx(int x, int y, const char* text,
                  float xscale, float yscale, float angle, 
                  int h_align=0, int v_align=0) -> void {
    context_window_->drawer->DrawTextEx(x, y, text, xscale, yscale,
                                        angle, h_align, v_align);
  }
   
  inline auto DrawSprite(int x, int y, const char* name,
                         float xscale, float yscale) -> void {
    context_window_->drawer->DrawSprite(x, y, name, xscale, yscale);
  }

  inline auto DrawSpriteEx(int x, int y, const char* name,
                    float xscale, float yscale, float angle,
                    int h_align=0, int v_align=0)   -> void {
    context_window_->drawer->DrawSpriteEx(x, y, name, xscale, yscale,
                                          angle, h_align, v_align);
  }

  inline auto DrawMesh(const char* name, math::Vec3 pos, math::Vec3 rot, math::Vec3 scale) -> void {
    renderer3d_->AddMeshToRenderHeap(name, pos, rot, scale);
  }

  inline auto DrawSelf() -> void {
    if (meshname[0] != '\0') {
      DrawMesh(meshname, pos, rot, scale);
    }
  }

private:
  // Events
  virtual auto OnCreate()  -> void {}

  virtual auto OnUpdate()  -> void {}

  virtual auto OnDestroy() -> void {}

  virtual auto OnDraw()    -> void { DrawSelf(); }
  
  // Vars
  const char *type_ = "";

  Scene *scene_                = nullptr;
  sdl::Window *context_window_ = nullptr;
  gfx::Renderer3D *renderer3d_ = nullptr;
};

}
