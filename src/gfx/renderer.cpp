#include "renderer.h"

#include <iostream>

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
  auto Renderer::set_camera_transform(const vec3 &pos, const vec3 &rot) -> void {
    _cam.pos = pos;
    _cam.rot = rot;
  }

  auto Renderer::render_add_mesh(const char* meshname, const vec3 &pos, const vec3 &rot) -> void {
    _meshesToDraw.push_back( rend_info{_meshes[meshname], pos, rot} );
  }

  auto Renderer::render() -> void {
    matrix4x4 viewmat = _calc_view_matrix();
    vector<face> facesToDraw;

    // Render meshes
    for(auto &[mesh, pos, rot] : _meshesToDraw) {
      matrix4x4 transfmat = _calc_transform_matrix(pos, rot);

      // Transform verticies
      vec3 *transformedVerts = new vec3[mesh->verts.size()];
      for (int i = 0; i < mesh->verts.size(); ++i)
        transformedVerts[i] = mesh->verts[i] * transfmat;

      // Viewed verticies
      vec3 *viewedVerts = new vec3[mesh->verts.size()];
      for (int i = 0; i < mesh->verts.size(); ++i)
        viewedVerts[i] = transformedVerts[i] * viewmat;

      // Projected verticies
      vec3 *projectedVerts = new vec3[mesh->verts.size()];
      for (int i = 0; i < mesh->verts.size(); ++i)
        projectedVerts[i] = viewedVerts[i] * _projmat;

      // Filter faces
      for(auto &face : mesh->faces) {
        vec3 vverts[] = {
          viewedVerts[face.verts[0]],
          viewedVerts[face.verts[1]],
          viewedVerts[face.verts[2]]
        };

        // Skip if normal look in other direction
        vec3 nrml = vec3::cross(vverts[2] - vverts[0], vverts[2] - vverts[1]);
        nrml.normalize();

        if (vec3::dot(nrml, vec3::normalized(vverts[0])) >= 0.0f) { continue; }

        // Push face
        facesToDraw.push_back(face);
      }

      _sort_faces_by_distance(&facesToDraw, viewedVerts);

      // Draw faces on screen
      for (auto &face : facesToDraw) {
        vec3 tverts[] = {
          transformedVerts[face.verts[0]],
          transformedVerts[face.verts[1]],
          transformedVerts[face.verts[2]]
        };

        vec3 verts[] = {
          projectedVerts[face.verts[0]],
          projectedVerts[face.verts[1]],
          projectedVerts[face.verts[2]]
        };

        // Scale to screen
        for(int i = 0; i < 3; ++i) {
          verts[i].x += 1;
          verts[i].y += 1;

          verts[i].x *= 0.5 * _window->resw;
          verts[i].y *= -0.5 * _window->resh;
          verts[i].y += _window->resh;
        }

        // Calc light
        vec3 nrml = vec3::cross(tverts[2] - tverts[0], tverts[2] - tverts[1]);
        nrml.normalize();

        vec3 sundir{1, 1, 1}; 
        sundir.normalize();
        float lightk = (1 + vec3::dot(nrml, sundir)) / 2;

        _window->set_draw_color(255 * lightk, 255 * lightk, 255 * lightk, 255);

        // Draw lines
        _window->draw_triangle(
          verts[0].x, verts[0].y,
          verts[1].x, verts[1].y,
          verts[2].x, verts[2].y
        );
      }
      facesToDraw.clear();
    }
    _meshesToDraw.clear();
  }

  // internal methods
  // ~ render
  auto Renderer::_calc_view_matrix() -> matrix4x4 {
    matrix4x4 resmat, rotmat;

    rotmat.set_rot(_cam.rot);
    resmat.look_at(_cam.pos, _cam.pos + rotmat.forward(), _projmat.up());

    return resmat;
  }

  auto Renderer::_calc_transform_matrix(const vec3 &pos, const vec3 &rot) -> matrix4x4 {
    matrix4x4 rotmat, translmat, resmat;

    rotmat.set_rot(rot);
    translmat.set_translation(pos);

    resmat.set_identity();
    resmat *= rotmat;
    resmat *= translmat;

    return resmat;
  }

  auto Renderer::_sort_faces_by_distance(vector<face> *faces, math::vec3 verts[]) -> void {
    sort(
      faces->begin(),
      faces->end(),
      [=](face &f1, face &f2) {
        float z1 = (verts[f1.verts[0]].z + verts[f1.verts[1]].z + verts[f1.verts[2]].z) / 3;
        float z2 = (verts[f2.verts[0]].z + verts[f2.verts[2]].z + verts[f2.verts[2]].z) / 3;
        return z1 > z2;
      }
    );
  }
}
