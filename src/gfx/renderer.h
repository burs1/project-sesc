#pragma once

#include <map>
#include <algorithm>
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
    auto set_perspective(float, float, float) -> void;

    auto set_sun_direction(const math::vec3&) -> void;

    // ~ resources
    auto load_mesh(const char*, const char*)  -> void;

    // ~ draw
    auto set_camera_transform(const math::vec3&, const math::vec3&) -> void;

    auto render_add_mesh(const char*, const math::vec3&, const math::vec3&) -> void;

    auto render() -> void;

  private:
    // internal methods
    // ~ render
    auto _calc_view_matrix() -> math::matrix4x4;
    
    auto _render_mesh(const mesh*, const math::vec3&, const math::vec3&, const math::matrix4x4&) -> void;

    auto _calc_transform_matrix(const math::vec3&, const math::vec3&) -> math::matrix4x4;

    auto _find_plane_intersection_point(const math::vec3&, const math::vec3&, const math::vec3&, const math::vec3&) -> math::vec3;
    
    auto _face_clip_against_plane(const math::vec3&, const math::vec3&, const math::vec3[3], math::vec3[3], math::vec3[3]) -> int;

    struct rend_info {
      mesh *mesh;
      math::vec3 pos, rot;
    };

    // vars
    Window *_window;
    struct { math::vec3 pos, rot; } _cam;

    math::vec3 _sundir;

    std::map< const char*, mesh* > _meshes;

    std::vector< rend_info > _meshesToDraw;
    math::matrix4x4 _projmat;
  };
}
