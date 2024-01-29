#include <SDL_scancode.h>
#include <cmath>
#include <string>

#include "math/matrix4x4.h"
#include "math/vec3.h"
#include "gmpl/scene.h"
#include "gmpl/entity.h"
#include "window/window.h"
#include "gfx/renderer3d.h"
#include "utils/funcs.h"
#include "logger/logger.h"

using namespace std;
using namespace eng;

class Ship : public gmpl::Entity {
private:
  void OnCreate() override {
    //SetCameraTransform(&pos, &rot);
  }

  void OnUpdate() override {
    float dt = GetDeltaTime();

    // Input
    int thrust_axis = IsKeyDown(SDL_SCANCODE_W) - IsKeyDown(SDL_SCANCODE_S);
    int strafe_axis = IsKeyDown(SDL_SCANCODE_D) - IsKeyDown(SDL_SCANCODE_A);

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

    vel = rotmat.Forward() * math::Vec3(strafe_axis, 0.0f, thrust_axis) * max_thrust;
    pos += vel * dt;
  }

  void OnDrawGUI() override {
    DrawText(0, 0, std::to_string(rot.y).c_str());
    DrawText(0, 75, std::to_string(pos.x).c_str());
  }

  float acceleration_speed = 1.0f;
  float decceleration_speed = 0.25f;
  float mobility = 0.5f;

  float max_thrust = 15.0f;
  float max_tilt   = 2.0f;
  float max_yaw    = 2.0f;
  float max_roll   = 2.0f;

  float tilt   = 0.0f;
  float yaw    = 0.0f;
  float roll   = 0.0f;

  math::Vec3 vel;
};

class Planet : public gmpl::Entity {
private:
  void OnUpdate() override {
    rot.y += GetDeltaTime() * 2;
  }
};

int main (int argc, char *argv[]) {
  // Init systems
  window::Window::Create("starfighters", 1280, 720);

  window::Window *window = window::Window::GetInstance();
  window::Drawer *drawer = window::Drawer::GetInstance();

  drawer->LoadTexture("assets/sprites/rock.png", "rock");

  drawer->LoadFont("assets/fonts/superstar.ttf", "main", 16);
  drawer->SetDrawFont("main");

  gfx::Renderer3D::Create();
  gfx::Renderer3D *renderer3d = gfx::Renderer3D::GetInstance();

  gmpl::Scene::Create();
  gmpl::Scene *scene = gmpl::Scene::GetInstance();

  // Load assets
  renderer3d->LoadMesh("assets/models/cube.obj", "planet");

  auto planet = scene->InstantiateEntity<Planet>("planet");
  planet->pos = math::Vec3(0, 0, 100);


  scene->InstantiateEntity<Ship>();

  // Delta time 
  int updateDelay = 16; //16 ticks ~ 61 fps
  Uint32 lastUpdateTime = window->GetTicks();
  Uint32 nextUpdateTime = window->GetTicks();

  while (not window->IsCloseRequested()) {
    // Hold update untill it's time
    Uint32 currentTime = window->GetTicks();
    if (currentTime < nextUpdateTime) { continue; }

    lastUpdateTime = currentTime;
    nextUpdateTime = currentTime - currentTime % updateDelay + updateDelay;

    window->PollEvents();
    scene->Update();
    window->UpdateSurface();
  }

  // Clean up
  gmpl::Scene::Destroy();
  gfx::Renderer3D::Destroy();
  window::Window::Destroy();

  return 0;
}
