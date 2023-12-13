#include "renderer.h"

#include <iostream>

using namespace std;
using namespace engine::math;

namespace engine::gfx {
  Renderer::Renderer(Window* window)
    : _window(window) {
      set_perspective(70, 0.1f, 1000);
      set_sun_direction(vec3(1, 1, 1));
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

  auto Renderer::set_sun_direction(const math::vec3 &dir) -> void {
    _sundir = dir;
    _sundir.normalize();
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
    for(auto [mesh, pos, rot] : _meshesToDraw) { _render_mesh(mesh, pos, rot, viewmat); }
    _meshesToDraw.clear();
  }

  // internal methods
  // ~ render
  auto Renderer::_render_mesh(const mesh *mesh, const vec3 &pos, const vec3 &rot, const matrix4x4 &viewmat) -> void {
    matrix4x4 transfmat = _calc_transform_matrix(pos, rot);
    std::vector<face> facesToDraw;
    
    vec3 *transfVerts = new vec3[mesh->verts.size()];

    // Filter faces
    for (auto [vertsInds, clr] : mesh->faces) {
      vec3 verts[] = {
        mesh->verts[vertsInds[0]],
        mesh->verts[vertsInds[1]],
        mesh->verts[vertsInds[2]]
      };

      // Translate and rot verts
      for (int i = 0; i < 3; ++i) { verts[i] *= transfmat; }

      // calc light
      vec3 nrml = vec3::cross(verts[1] - verts[0], verts[2] - verts[0]);
      nrml.normalize();
      float lightk = (1 + vec3::dot(nrml, _sundir)) / 2;
      
      // Transform verts to view perspective
      for (int i = 0; i < 3; ++i) { verts[i] *= viewmat; }

      // Skip face if it's normal points at opposite side
      nrml = vec3::cross(verts[1] - verts[0], verts[2] - verts[0]);
      nrml.normalize();
      if (vec3::dot(nrml, viewmat.forward()) > 0.0f) { continue; }

      // copy verts
      for (int i = 0; i < 3; ++i) { transfVerts[vertsInds[i]] = verts[i]; }

      facesToDraw.push_back( face{
        {vertsInds[0], vertsInds[1], vertsInds[2]},
        {int(clr[0] * lightk), int(clr[1] * lightk), int(clr[2] * lightk)}
      });
    }

    // sort faces
    sort(
      facesToDraw.begin(),
      facesToDraw.end(),
      [=](face &f1, face &f2) {
        float z1 = transfVerts[f1.verts[0]].z;// + transfVerts[f1.verts[1]].z + transfVerts[f1.verts[2]].z;
        float z2 = transfVerts[f2.verts[0]].z;// + transfVerts[f2.verts[2]].z + transfVerts[f2.verts[2]].z;
        return z1 > z2;
      }
    );
    // Draw faces
    for (auto [vertsInds, clr] : facesToDraw) {
      vec3 verts[] = {
        transfVerts[vertsInds[0]],
        transfVerts[vertsInds[1]],
        transfVerts[vertsInds[2]]
      };

      // project verts
      for (int i = 0; i < 3; ++i) {
        //int ClippedFaces = 0;
        //face clipped[2];
        //vec3 verts(vertss);
        //ClippedFaces = _face_clip_against_plane(vec3(0.0f, 0.0f, 0.1f), vec3(0.0f, 0.0f, 1.0f), verts[i], clipped[0], clipped[1]);
        //for (int j = 0; j < ClippedFaces; ++j) {
        //verts[i] = clipped[j];
        verts[i] *= _projmat;

        verts[i].x += 1;
        verts[i].y += 1;

        verts[i].x *= 0.5 * _window->resw;
        verts[i].y *= -0.5 * _window->resh;

        verts[i].y += _window->resh;
        //}
      }

      // draw face
      _window->set_draw_color(clr[0], clr[1], clr[2], 255);
      _window->draw_triangle(
        verts[0].x, verts[0].y,
        verts[1].x, verts[1].y,
        verts[2].x, verts[2].y
      );
    }

    delete[] transfVerts;
  }

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

  auto _find_plane_intersection_point(const vec3 &plane_p, const vec3 &plane_n, const vec3 &start, const vec3 &end) -> vec3 {
    float plane_d = vec3::dot(plane_n, plane_p);
    float start_d = vec3::dot(start, plane_n);
    float end_d = vec3::dot(end, plane_n);
    float t = (plane_d - start_d) / (end_d - start_d);
    vec3 start_to_end = end - start;
    vec3 inter_line = start_to_end * t;
    return start + inter_line;
  }

auto _face_clip_against_plane(const vec3 &plane_p, const vec3 &plane_n, vec3 in[3], vec3 out1[3], vec3 out2[3]) -> int {
   
   vec3* inside[3]; int insideCount = 0;
   vec3* outside[3]; int outsideCount = 0;
 
   float d0 = vec3::dot(plane_p, in[0]) - vec3::dot(plane_n, plane_p);
   float d1 = vec3::dot(plane_p, in[1]) - vec3::dot(plane_n, plane_p);
   float d2 = vec3::dot(plane_p, in[2]) - vec3::dot(plane_n, plane_p);
   
   if (d0 >= 0) { inside[insideCount++] = &in[0]; }
   else { outside[outsideCount++] = &in[0]; }
   if (d1 >= 0) { inside[insideCount++] = &in[1]; }
   else { outside[outsideCount++] = &in[1]; }
   if (d2 >= 0) { inside[insideCount++] = &in[2]; }
   else { outside[outsideCount++] = &in[2]; }
 
   if (insideCount == 0)
   {
     return 0;
   }
 
   if (insideCount == 3)
   {
     out1[0] = in[0];
     out1[1] = in[1];
     out1[2] = in[2];
 
     return 1;
   }
 
   if (insideCount == 1 && outsideCount == 2)
   {
     out1[0] = *inside[0];
     out1[1] = _find_plane_intersection_point(plane_p, plane_n, *inside[0], *outside[0]);
     out1[2] = _find_plane_intersection_point(plane_p, plane_n, *inside[0], *outside[1]);
 
     return 1;
   }
 
   if (insideCount == 2 && outsideCount == 1)
   {
     out1[0] = *inside[0];
     out1[1] = *inside[1];
     out1[2] = _find_plane_intersection_point(plane_p, plane_n, *inside[0], *outside[0]);
 
     out2[0] = *inside[1];
     out2[1] = out1[2];
     out2[2] = _find_plane_intersection_point(plane_p, plane_n, *inside[1], *outside[0]);
 
     return 2;
   }
 
   return 0;
}
}
