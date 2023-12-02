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
    rotVel.x = lerp(rotVel.x, tiltAxis * rotSp, rotAcc * deltaTime);
    rotVel.y = lerp(rotVel.y, yawAxis * rotSp, rotAcc * deltaTime);
    rotVel.z = lerp(rotVel.z, rollAxis * rotSp, rotAcc * deltaTime);

    // rotate
    rot += rotVel * deltaTime;
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
  Renderer *renderer = new Renderer(window, campos, camrot);
  float deltaTime = 0.016;

  Scene *scene = new Scene(window, renderer, deltaTime);

  // Load assets
  window->load_sprite("assets/logo.png", "logo");
  window->load_font("assets/superstar.ttf", 16, "super");
  renderer->load_mesh("assets/cube.obj", "ship");

  scene->instantiate_entity<Ship>("ship");

  // Delta time 
  int updateDelay = 16; //16 ticks ~ 60 fps
  Uint32 lastUpdateTime = window->get_ticks();
  Uint32 nextUpdateTime = window->get_ticks();

  vec3 shippos(0, 0, 8);
  vec3 shiprot(0, 0, 0);

  window->set_font("super");
  window->set_draw_color(57, 156, 255, 255);

  while (lastUpdateTime < 10000) {
    // Hold update untill it's time
    Uint32 currentTime = window->get_ticks();
    if (currentTime < nextUpdateTime) { continue; }

    lastUpdateTime = currentTime;
    nextUpdateTime = currentTime - currentTime % updateDelay + updateDelay;

    float timeScaled = currentTime / 1000.0f;

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
