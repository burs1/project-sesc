#include <cmath>
#include "vec3.h"
#include "scene.h"
#include "entity.h"
#include "window.h"
#include "renderer.h"

using namespace std;
using namespace engine;
using namespace engine::gfx;
using namespace engine::math;
using namespace engine::gmpl;

class Player : public Entity {
private:
  void on_update() override {
    // Input
    float thrustAxis = input_axis(SDL_SCANCODE_S, SDL_SCANCODE_W) * get_delta_time();
    int mouseX, mouseY;
    get_mouse_pos(mouseX, mouseY, true);

    float yawAxis = 0;
    if (abs(mouseX) > 150) { yawAxis = (mouseX > 0 ? 1 : -1); }

    // Rotate
    rot.y += yawAxis * get_delta_time();

    matrix4x4 rotmat;
    rotmat.set_rot(rot);
    pos += rotmat.forward() * thrust;

    set_camera_transform(pos, rot);
  }

  float thrust = 0;
};

class Planet : public Entity {
private:
  void on_update() override {
    rot.y += get_delta_time() * 2;
  }
};

int main (int argc, char *argv[]) {
  // Init systems
  Window *window = new Window("starfighters", 320, 180, 1280, 720, false);
  Renderer *renderer = new Renderer(window);
  float deltaTime = 0.016;

  Scene *scene = new Scene(window, renderer, deltaTime);

  // Load assets
  renderer->load_mesh("assets/models/ship.obj", "ship");
  renderer->load_mesh("assets/models/planet.obj", "planet");


  auto planet = scene->instantiate_entity<Planet>("planet");
  planet->pos = vec3(0, 0, 100);
  planet->rot = vec3(0.2, 0.2, 0);

  scene->instantiate_entity<Player>();

  // Delta time 
  int updateDelay = 16; //16 ticks ~ 61 fps
  Uint32 lastUpdateTime = window->get_ticks();
  Uint32 nextUpdateTime = window->get_ticks();

  while (window->isOpened) {
    // Hold update untill it's time
    Uint32 currentTime = window->get_ticks();
    if (currentTime < nextUpdateTime) { continue; }

    deltaTime = (currentTime - lastUpdateTime) / 1000.0f;

    lastUpdateTime = currentTime;
    nextUpdateTime = currentTime - currentTime % updateDelay + updateDelay;

    window->update_events();
    scene->update();
    renderer->render();
    window->update_surface();
  }

  // Clean up
  delete scene;
  delete renderer;
  delete window;

  return 0;
}
