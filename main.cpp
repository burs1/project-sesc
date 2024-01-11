#include <cmath>
#include <iostream>

#include "math/vec3.h"
#include "gmpl/scene.h"
#include "gmpl/entity.h"
#include "window/window.h"
#include "gfx/renderer3d.h"

using namespace std;
using namespace eng;

class Player : public gmpl::Entity {
private:
  void OnUpdate() override {
    // Input
    float forw_axis = (IsKeyDown(SDL_SCANCODE_W) - IsKeyDown(SDL_SCANCODE_S)) * GetDeltaTime();
    float right_axis = (IsKeyDown(SDL_SCANCODE_D) - IsKeyDown(SDL_SCANCODE_A)) * GetDeltaTime() * 5.0f;

    float rot_y = (IsKeyDown(SDL_SCANCODE_E) - IsKeyDown(SDL_SCANCODE_Q)) * GetDeltaTime() * 5.0f;

    // Rotate
    rot.y += rot_y;

    math::Matrix4x4 rotmat;
    rotmat.SetRotation(rot);
    pos += rotmat.Forward() * forw_axis * 10.0f;
    pos += rotmat.Right() * right_axis * 5.0f;

    SetCameraTransform(pos, rot);
  }

  float thrust = 0;
};

class Planet : public gmpl::Entity {
private:
  void OnUpdate() override {
    rot.y += GetDeltaTime() * 2;
  }
};

int main (int argc, char *argv[]) {
  // Init systems
  sdl::Window *window = new sdl::Window("starfighters", 1280, 720, false);
  window->drawer->SetRenderLogicalSize(320, 180);

  gfx::Renderer3D *renderer3d = new gfx::Renderer3D(window);

  float delta_time = 0.016;

  gmpl::Scene *scene = new gmpl::Scene(window, renderer3d, delta_time);

  // Load assets
  renderer3d->LoadMesh("assets/models/planet.obj", "planet");

  auto planet = scene->InstantiateEntity<Planet>("planet");
  planet->pos = math::Vec3(0, 0, 100);
  planet->rot = math::Vec3(0.2, 0.2, 0);

  planet = scene->InstantiateEntity<Planet>("planet");
  planet->pos = math::Vec3(0, 0, 400);
  planet->rot = math::Vec3(0.2, 0.5, 0);
  planet->scale = math::Vec3(5.0f, 1.0f, 5.0f);

  scene->InstantiateEntity<Player>();

  // Delta time 
  int updateDelay = 16; //16 ticks ~ 61 fps
  Uint32 lastUpdateTime = window->GetTicks();
  Uint32 nextUpdateTime = window->GetTicks();

  while (not window->is_close_requested) {
    // Hold update untill it's time
    Uint32 currentTime = window->GetTicks();
    if (currentTime < nextUpdateTime) { continue; }

    delta_time = (currentTime - lastUpdateTime) / 1000.0f;

    lastUpdateTime = currentTime;
    nextUpdateTime = currentTime - currentTime % updateDelay + updateDelay;

    window->UpdateEvents();
    scene->Update();
    renderer3d->RenderHeap();
    window->UpdateSurface();
  }

  // Clean up
  delete scene;
  delete renderer3d;
  delete window;

  return 0;
}
