#pragma once

#include <map>
#include <cmath>
#include <vector>
#include "mesh.h"
#include "vec3.h"
#include "matrix4x4.h"
#include "window/window.h"

namespace engine::gfx {

  class Renderer {
  public:
    Renderer(Window*);

    ~Renderer();

    // methods
    // ~ set up
    auto set_perspective(float, float, float)           -> void;

    // ~ resources
    auto load_mesh(const char*, const char*)            -> void;

    // ~ draw
    auto set_camera_transform(math::vec3, math::vec3)   -> void;

    auto render_add_mesh(const char*, math::vec3, math::vec3) -> void;

    auto render() -> void;

  private:
    struct rend_info {
      mesh *mesh;
      math::vec3 pos, rot;
    };


    // vars
    Window *_window;
    struct { math::vec3 pos, rot; } _cam;

    std::map< const char*, mesh* > _meshes;

    std::vector< rend_info > _meshesToDraw;
    math::matrix4x4 _projmat;

  };
}
