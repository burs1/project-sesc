#include "renderer.h"
#include <stdexcept>

using namespace std;
using namespace engine::math;

namespace engine::gfx {
  Renderer::Renderer(Window* window)
    : _window(window) {
      set_perspective(70, 0.1f, 1000);
  }

  Renderer::~Renderer() {
    // Destroy all loaded meshed
    for (auto [name, mesh] : _meshes) { delete mesh; }
    _meshes.clear();
  }

  // methods
  // ~ set up
  auto Renderer::set_perspective(float fov, float near, float far) -> void {
    float aspratio = (float)_window->resh / _window->resw;
    _projmat.set_perspective(fov, aspratio, near, far);
  }


  // ~ resources
  auto Renderer::load_mesh(const char *path, const char *name) -> void {
    // Check if mesh with such name already exists
    if (_meshes.contains(name))
      throw runtime_error("Mesh with name \"" + string(name) + "\" already exists");

    _meshes[name] = new mesh(path);
  }


  // ~ draw
  auto Renderer::set_camera_transform(vec3 pos, vec3 rot) -> void {
    _cam.pos = pos; _cam.rot = rot;
  }

  auto Renderer::render_add_mesh(const char* meshname, vec3 pos, vec3 rot) -> void {
    _meshesToDraw.push_back( rend_info{_meshes[meshname], pos, rot} );
  }

  auto Renderer::render() -> void {
    matrix4x4 viewmat, viewrotmat;
    viewrotmat.set_rot(_cam.rot);
    viewmat.look_at(_cam.pos, _cam.pos + vec3(0, 0, 1) * viewrotmat, _projmat.up());

    for(auto [mesh, pos, rot] : _meshesToDraw) {
      // Prepare matricies
      matrix4x4 rotmat, transmat, worldmat;

      rotmat.set_rot(rot);
      transmat.set_translation(pos);

      worldmat.set_identity();
      worldmat *= rotmat;
      worldmat *= transmat;

      // Draw faces
      for(auto [p1, p2, p3] : mesh->faces) {
        vec3 curVerts[] = {mesh->verts[p1], mesh->verts[p2], mesh->verts[p3]};

        // Transform, view and project verticies
        for(int i = 0; i < 3; ++i) {
          curVerts[i] *= worldmat;
          curVerts[i] *= viewmat;
          curVerts[i] = curVerts[i] * _projmat;
        }

        // Find normal
        vec3 nrml;
        nrml = vec3::cross(curVerts[2] - curVerts[0], curVerts[2] - curVerts[1]);
        nrml.normalize();

        // Don't draw if normal look in other direction
        if (vec3::dot(nrml, vec3::normalized(curVerts[0])) >= 0.0f) { continue; }

        // Scale to screen
        for(int i = 0; i < 3; ++i) {
          curVerts[i].x += 1;
          curVerts[i].y += 1;

          curVerts[i].x *= 0.5 * _window->resw;
          curVerts[i].y *= -0.5 * _window->resh;
        }

        // Draw lines
        for (int i = 0; i < 3; ++i)
          _window->draw_line(
            curVerts[i].x, _window->resh + curVerts[i].y,
            curVerts[(i + 1)%3].x, _window->resh + curVerts[(i + 1)%3].y
          );
      }
    }

    _meshesToDraw.clear();
  }
}
