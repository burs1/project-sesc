#include <SDL_scancode.h>
#include <cmath>
#include <iostream>
#include <string>

#include "math/matrix4x4.h"
#include "math/vec3.h"
#include "gmpl/scene.h"
#include "gmpl/entity.h"
#include "window/window.h"
#include "gfx/renderer3d.h"
#include "utils/funcs.h"

using namespace std;
using namespace eng;

class Ship : public gmpl::Entity {
private:
  void OnUpdate() override {
    float dt = GetDeltaTime();

    // Input
    int thrust_axis = IsKeyDown(SDL_SCANCODE_W) - IsKeyDown(SDL_SCANCODE_S);

    int tilt_axis   = IsKeyDown(SDL_SCANCODE_DOWN) - IsKeyDown(SDL_SCANCODE_UP);
    int yaw_axis    = IsKeyDown(SDL_SCANCODE_RIGHT) - IsKeyDown(SDL_SCANCODE_LEFT);
    int roll_axis   = IsKeyDown(SDL_SCANCODE_Q) - IsKeyDown(SDL_SCANCODE_E);

    // Rotate
    tilt = std::lerp(tilt, tilt_axis * max_tilt, mobility * dt);
    yaw = std::lerp(yaw, yaw_axis * max_yaw, mobility * dt);
    roll = std::lerp(roll, roll_axis * max_roll, mobility * dt);

    rot += math::Vec3(tilt, yaw, roll) * dt;

    // Move
    math::Matrix4x4 rotmat;
    rotmat.SetRotation(rot);

    vel = math::Vec3::Lerp(
      vel,
      rotmat.Forward() * thrust_axis * max_thrust,
      (thrust_axis != 0 ? acceleration_speed : decceleration_speed) * dt);

    pos += vel * dt;

    // Camera
    math::Vec3 shake = rotmat.Right() * vel.Magnitude()
                       * (utils::random_range(1, 100) / 100.0f)
                       * shake_magnitude;
    SetCameraTransform(pos + shake, rot);

    SetCameraFOV(70 + 10 * (vel.Magnitude() / max_thrust));
  }

  float acceleration_speed = 1.0f;
  float decceleration_speed = 0.25f;
  float mobility = 0.5f;

  float max_thrust = 5.0f;
  float max_tilt   = 2.0f;
  float max_yaw    = 2.0f;
  float max_roll   = 2.0f;

  float tilt   = 0.0f;
  float yaw    = 0.0f;
  float roll   = 0.0f;

  math::Vec3 vel;

  float shake_magnitude = 0.1f;
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

  window->drawer->LoadSprite("assets/sprites/rock.png", "rock");

  window->drawer->LoadFont("assets/fonts/superstar.ttf", "main", 16);
  window->drawer->SetDrawFont("main");

  gfx::Renderer3D *renderer3d = new gfx::Renderer3D(window);

  float delta_time = 0.016;

  gmpl::Scene *scene = new gmpl::Scene(window, renderer3d, delta_time);

  // Load assets
  renderer3d->LoadMesh("assets/models/textured-cube.obj", "planet" ,"rock");

  auto planet = scene->InstantiateEntity<Planet>("planet");
  planet->pos = math::Vec3(0, 0, 20);
  planet->rot = math::Vec3(0.2, 0.2, 0);

  scene->InstantiateEntity<Ship>();

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
    window->UpdateSurface();
  }

  // Clean up
  delete scene;
  delete renderer3d;
  delete window;

  return 0;
}
