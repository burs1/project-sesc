#pragma once

#include "gmpl/scene.h"
#include "gmpl/entity.h"
#include "gmpl/component.h"
#include "gmpl/components/transform.h"
#include "window/input.h"
#include "window/audio.h"
#include "window/drawer.h"

namespace eng::gmpl {

class Scene;

class Behaviour : public DynamicComponent {
friend Scene;
 protected:
  inline auto CreateEntity() -> Entity* {
    return scene->CreateEntity();
  }


  inline auto DestroyEntity(Entity* entity) -> void {
    scene->DestroyEntity(entity);
  }


  template<typename T>
  inline auto AddComponent() -> T* {
    return entity->AddComponent<T>();
  }


  inline auto DestroyComponent(Component* component) -> void {
    scene->DestroyComponent(component);
  }


  inline auto SetCursorLock(bool lock) -> void {
    input->SetCursorLock(lock);
  }


  inline auto IsKeyDown(SDL_Scancode key) const -> bool {
    return input->IsKeyDown(key);
  }


  inline auto IsKeyPressed(SDL_Scancode key) const -> bool {
    return input->IsKeyPressed(key);
  }


  inline auto IsKeyReleased(SDL_Scancode key) const -> bool {
    return input->IsKeyPressed(key);
  }


  inline auto IsMouseButtonDown(int button) const -> bool {
    return input->IsMouseButtonDown(button);
  }


  inline auto IsMouseButtonPressed(int button) const -> bool {
    return input->IsMouseButtonPressed(button);
  }


  inline auto IsMouseButtonReleased(int button) const -> bool {
    return input->IsMouseButtonReleased(button);
  }


  inline auto GetMousePosition(int *x, int *y) const -> void {
    input->GetMousePosition(x, y);
  }


  inline auto GetMouseDelta(int *x, int *y) const -> void {
    input->GetMouseDelta(x, y);
  }


  inline auto SetGlobalVolume(float volume) -> void {
    audio->SetGlobalVolume(volume);
  }


  inline auto SetListenerTransform(const math::Vec3 *pos,
                                   const math::Vec3 *rot) -> void {
    audio->SetListenerTransform(pos, rot);
  }


  inline auto PlaySound(const char *name, float volume=1.0f,
                        float pan=0.0f, float pitch=1.0f) -> void {
    audio->PlaySound(name, volume, pan, pitch);
  }


  inline auto PlaySound3D(const char *name, math::Vec3 pos,
                          float range, float volume=1.0f) -> void {
    audio->PlaySound3D(name, pos, range, volume);
  }

  
  inline auto SetClearColor(Uint8 r, Uint8 g, Uint8 b) -> void {
    drawer->SetClearColor(r, g, b);
  }


  inline auto SetDrawColor(Uint8 r, Uint8 g, Uint8 b) -> void {
    drawer->SetDrawColor(r, g, b);
  }


  inline auto SetDrawFont(const char *name) -> void {
    drawer->SetDrawFont(name);
  }


  inline auto GetTexture(const char *name) -> const window::Texture* {
    return drawer->GetTexture(name);
  }

  inline auto DrawPoint(int x, int y) -> void {
    drawer->DrawPoint(x, y);
  }

  inline auto DrawPointN(float x, float y) -> void {
    drawer->DrawPointN(x, y);
  }

  inline auto DrawLine(int x1, int y1, int x2, int y2) -> void {
    drawer->DrawLine(x1, y1, x2, y2);
  }

  inline auto DrawLineN(float x1, float y1, float x2, float y2) -> void {
    drawer->DrawLineN(x1, y1, x2, y2);
  }

  inline auto DrawTriangle(int x1, int y1, int x2, int y2, int x3, int y3,
                           bool fill=true) -> void {
    drawer->DrawTriangle(x1, y1, x2, y2, x3, y3, fill);
  }

  inline auto DrawTriangleN(float x1, float y1, float x2, float y2, 
                            float x3, float y3, bool fill=true) -> void {
    drawer->DrawTriangleN(x1, y1, x2, y2, x3, y3, fill);
  }

  inline auto DrawRect(int x1, int y1, int x2, int y2,
                       bool fill=true) -> void {
    drawer->DrawRect(x1, y1, x2, y2, fill);
  }

  inline auto DrawRectN(float x1, float y1, int x2, int y2,
                        bool fill=true) -> void {
    drawer->DrawRectN(x1, y1, x2, y2, fill);
  }

  inline auto DrawTexture(int x, int y, const char *name,
                          float xscale, float yscale) -> void {
    drawer->DrawTexture(x, y, name, xscale, yscale);
  }

  inline auto DrawTextureN(float x, float y, const char *name,
                           float xscale, float yscale) -> void {
    drawer->DrawTextureN(x, y, name, xscale, yscale);
  }

  inline auto DrawTextureEx(int x, int y, const char *name,
                            float xscale, float yscale, float angle,
                            int halign=0, int valign=0) -> void {
    drawer->DrawTextureEx(x, y, name, xscale, yscale,
                           angle, halign, valign);
  }

  inline auto DrawTextureExN(float x, float y, const char *name,
                             float xscale, float yscale, float angle,
                             int halign=0, int valign=0) -> void {
    drawer->DrawTextureExN(x, y, name, xscale, yscale,
                           angle, halign, valign);
  }

  inline auto DrawText(int x, int y, const std::string &text,
                       float xscale, float yscale) -> void {
    drawer->DrawText(x, y, text, xscale, yscale);
  }

  inline auto DrawTextN(float x, float y, const std::string &text, 
                       float xscale, float yscale) -> void {
    drawer->DrawText(x, y, text, xscale, yscale);
  }

  inline auto DrawTextEx(int x, int y, const std::string &text,
                         float xscale, float yscale, float angle,
                         int halign=0, int valign=0) -> void {
    drawer->DrawTextEx(x, y, text, xscale, yscale,
                        angle, halign, valign);
  }

  inline auto DrawTextExN(float x, float y, const std::string &text,
                          float xscale, float yscale, float angle,
                          int halign=0, int valign=0) -> void {
    drawer->DrawTextExN(x, y, text, xscale, yscale,
                        angle, halign, valign);
  }

 private:

};

}

