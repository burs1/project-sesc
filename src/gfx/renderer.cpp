#include "renderer.h"
#include <stdexcept>

using namespace std;
using namespace engine::math;

namespace engine::gfx {
  Renderer::Renderer(Window* window, vec3 &campos, vec3 &camrot)
    : _window(window), campos(campos), camrot(camrot) {
      set_perspective(90, 0.1f, 1000);
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
  auto Renderer::render_add_mesh(const char* meshname, vec3 pos, vec3 rot) -> void {
    _meshesToDraw.push_back( rend_info{meshname, pos, rot} );
  }

  auto Renderer::render() -> void {
    for(auto [meshname, pos, rot] : _meshesToDraw) {
      mesh *mesh = _meshes[meshname];

      matrix4x4 rotmatX, rotmatY, rotmatZ;
      rotmatX.set_rotX(rot.x);
      rotmatY.set_rotY(rot.y);
      rotmatZ.set_rotZ(rot.z);

      // Draw faces
      for(auto [p1, p2, p3] : mesh->faces) {
        vec3 curVerts[] = {mesh->verts[p1], mesh->verts[p2], mesh->verts[p3]};

        // Rotate, translate and project verticies
        for(int i = 0; i < 3; ++i) {
          curVerts[i] *= rotmatX;
          curVerts[i] *= rotmatY;
          curVerts[i] *= rotmatZ;
          curVerts[i] += pos;
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
          curVerts[i].y *= 0.5 * _window->resh;
        }

        // Draw lines
        for (int i = 0; i < 3; ++i)
          _window->draw_line(
            curVerts[i].x, _window->resh - curVerts[i].y,
            curVerts[(i + 1)%3].x, _window->resh - curVerts[(i + 1)%3].y
          );
      }
    }

    _meshesToDraw.clear();
  }
}
