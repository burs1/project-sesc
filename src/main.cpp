#include <SDL_scancode.h>
#include <charconv>
#include <cmath>
#include <string>
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

class Ship : public Entity {
private:
  void on_create() override {
    pos.z = 5;
  }

  void on_update() override {
    float deltaTime = get_delta_time();

    // input
    float rollAxis = input_axis(SDL_SCANCODE_A, SDL_SCANCODE_D);
    float tiltAxis = input_axis(SDL_SCANCODE_S, SDL_SCANCODE_W);
    float yawAxis = input_axis(SDL_SCANCODE_Q, SDL_SCANCODE_E);

    // velocity change
    rotVel.y = lerp(rotVel.y, tiltAxis * rotSp, rotAcc * deltaTime);
    rotVel.z = lerp(rotVel.z, yawAxis * rotSp, rotAcc * deltaTime);
    rotVel.x = lerp(rotVel.x, rollAxis * rotSp, rotAcc * deltaTime);

    // rotate
    pos += rotVel * deltaTime;
  }

  void on_draw() override {
    draw_sprite("logo", 0, 0, 0.75, 0.75);
    draw_text_ex( to_string(rotVel.magnitude()).c_str() , 145, 140, 0.5, 0.5, rotVel.magnitude());
    render_self();
  }

  vec3 rotVel;
  float rotSp = 10;
  float rotAcc = 2;
  
};

int main (int argc, char *argv[]) {
  vec3 campos, camrot;

  // Init systems
  Window *window = new Window("starfighters", 320, 180, 1280, 720);
  Renderer *renderer = new Renderer(window);
  float deltaTime = 0.016;

  Scene *scene = new Scene(window, renderer, deltaTime);

  // Load assets
  window->load_sprite("assets/logo.png", "logo");
  window->load_font("assets/superstar.ttf", 16, "super");
  renderer->load_mesh("assets/ship.obj", "ship");

  scene->instantiate_entity<Ship>("ship");

  // Delta time 
  int updateDelay = 16; //16 ticks ~ 60 fps
  Uint32 lastUpdateTime = window->get_ticks();
  Uint32 nextUpdateTime = window->get_ticks();

  window->set_font("super");
  window->set_draw_color(156, 156, 255, 255);

  while (window->isOpened) {
    // Hold update untill it's time
    Uint32 currentTime = window->get_ticks();
    if (currentTime < nextUpdateTime) { continue; }

    deltaTime = (currentTime - lastUpdateTime) / 1000.0f;

    lastUpdateTime = currentTime;
    nextUpdateTime = currentTime - currentTime % updateDelay + updateDelay;

    window->update_events();

    campos.x += window->input_axis(SDL_SCANCODE_LEFT, SDL_SCANCODE_RIGHT) * deltaTime;
    campos.y += window->input_axis(SDL_SCANCODE_DOWN, SDL_SCANCODE_UP) * deltaTime;
    camrot.x += window->input_axis(SDL_SCANCODE_K, SDL_SCANCODE_I) * deltaTime;
    camrot.y += window->input_axis(SDL_SCANCODE_J, SDL_SCANCODE_L) * deltaTime;

    scene->update();
    renderer->set_camera_transform(campos, camrot);
    renderer->render();

    window->update_surface();
  }

  // Clean up
  delete scene;
  delete renderer;
  delete window;

  return 0;
}
