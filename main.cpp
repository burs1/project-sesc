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

class FreeCamera : public gmpl::Entity {
private:
  void OnCreate() override {
    SetCameraTransform(&pos, &rot);
  }

  void OnUpdate() override {
    float dt = GetDeltaTime();

    // Input
    int forward_axis = IsKeyDown(SDL_SCANCODE_W) - IsKeyDown(SDL_SCANCODE_S);
    int right_axis = IsKeyDown(SDL_SCANCODE_D) - IsKeyDown(SDL_SCANCODE_A);
    int up_axis = IsKeyDown(SDL_SCANCODE_SPACE) - IsKeyDown(SDL_SCANCODE_LCTRL);

    int tilt_axis   = IsKeyDown(SDL_SCANCODE_DOWN) - IsKeyDown(SDL_SCANCODE_UP);
    int yaw_axis    = IsKeyDown(SDL_SCANCODE_RIGHT) - IsKeyDown(SDL_SCANCODE_LEFT);
    int roll_axis   = IsKeyDown(SDL_SCANCODE_Q) - IsKeyDown(SDL_SCANCODE_E);

    int speed_up_key = IsKeyDown(SDL_SCANCODE_LSHIFT);

    // Rotate
    auto rot_delta = math::Vec3(tilt_axis, yaw_axis, roll_axis);
    rot += rot_delta * rotation_speed_ * dt;

    // Move
    math::Matrix4x4 rotmat;
    rotmat.SetRotation(rot);

    auto move_delta = math::Vec3(right_axis, 0.0f, forward_axis) * rotmat;
    move_delta += math::Vec3(0.0f, up_axis, 0.0f);
    pos += (move_delta * move_speed_) * (speed_up_key ? 10.0f : 1.0f) * dt;
  }

  void OnDraw() override {
    DrawText3D(
      math::Vec3(0.0f, 10.0f, 50.0f),
      math::Vec3(0.0f, 0.0f, 0.0f),
      math::Vec3(1.0f, 1.0f, 1.0f),
      "Hello world!",
      true);

    DrawTexture3D(
      math::Vec3(0.0f, 50.0f, 50.0f),
      math::Vec3(0.0f, 0.0f, 0.0f),
      math::Vec3(1.0f, 1.0f, 1.0f),
      "rock",
      true);
  }

  void OnDrawGUI() override {
    // Draw pos and rot
    SetDrawColor(255, 255, 255);
    DrawText(0, 660, "rotation: " + std::to_string(rot), 2.0f, 2.0f);
    DrawText(0, 690, "position: " + std::to_string(pos), 2.0f, 2.0f);
  }

  float rotation_speed_ = 3.0f;
  float move_speed_     = 3.0f;

  float axis_line_length_ = 50.0f;
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
  renderer3d->LoadMesh("assets/models/textured-cube.obj", "cube", "rock");

  auto planet = scene->InstantiateEntity<Planet>("cube");
  planet->pos = math::Vec3(0, 0, 20);


  scene->InstantiateEntity<FreeCamera>();

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
