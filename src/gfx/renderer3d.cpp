#include "gfx/renderer3d.h"

#include <iostream>

namespace eng::gfx {

Renderer3D::Renderer3D(sdl::Window* context_window)
  : context_window_(context_window) {
    SetPerspective(70, 0.1f, 1000);
    SetSunDirection(math::Vec3(1, 1, 1));
}


Renderer3D::~Renderer3D() {
  // Unload all loaded meshes
  for (auto [name, mesh] : meshes_) {
    delete mesh;
  }
}


// Methods
// ~ Set up
auto Renderer3D::SetPerspective(float fov, float near, float far) -> void {
  float aspratio = (float)context_window_->drawer->resh / context_window_->drawer->resw;
  projmat_.SetPerspective(fov, aspratio, near, far);
}


auto Renderer3D::SetSunDirection(const math::Vec3 &dir) -> void {
  sun_direction_ = dir;
  sun_direction_.Normalize();
}


// ~ resources
auto Renderer3D::LoadMesh(const char *path, const char *name) -> void {
  // Check if mesh with such name already exists
  if (meshes_.contains(name)) { 
    throw std::runtime_error("Mesh with name \"" + std::string(name) + "\" already exists");
  }

  meshes_[name] = new Mesh(path);
}


// ~ draw
auto Renderer3D::SetCameraTransform(const math::Vec3 &pos,
                                    const math::Vec3 &rot) -> void {
  cam_.pos = pos;
  cam_.rot = rot;
}

auto Renderer3D::AddMeshToRenderHeap(
    const char* meshname,
    const math::Vec3 &pos,
    const math::Vec3 &rot,
    const math::Vec3 &scale) -> void {
  render_heap_.push_back( RenderData{meshes_[meshname], pos, rot, scale} );
}

auto Renderer3D::RenderHeap() -> void {
  CalcViewMatrix();
  std::vector<RawTriangle> triangles_to_draw;

  // Filter triangles
  for(auto render_data : render_heap_) {
    FilterMeshTriangles(render_data, triangles_to_draw);
  }

  // Sort triangles
  sort(
    triangles_to_draw.begin(),
    triangles_to_draw.end(),
    [](RawTriangle &trng1, RawTriangle &trng2) {
      float z1 = trng1.verts[0].z + trng1.verts[1].z + trng1.verts[2].z;
      float z2 = trng2.verts[0].z + trng2.verts[1].z + trng2.verts[2].z;
      return z1 > z2;
    }
  );
  
  // Draw triangles
  DrawTriangles(triangles_to_draw);

  render_heap_.clear();
}

// Internal methods
// ~ Render
auto Renderer3D::CalcViewMatrix() -> void {
  math::Matrix4x4 rotmat;
  rotmat.SetRotation(cam_.rot);
  
  viewmat_.LookAt(cam_.pos, cam_.pos + rotmat.Forward(), projmat_.Up());
}


auto Renderer3D::CalcTransformMatrix(
    const math::Vec3& pos,
    const math::Vec3& rot) -> math::Matrix4x4 {
  math::Matrix4x4 rotmat, translmat;
  
  rotmat.SetRotation(rot);
  translmat.SetTranslation(pos);
  
  return rotmat * translmat;
}


auto Renderer3D::FilterMeshTriangles(
    const RenderData& render_data,
    std::vector<RawTriangle>& triangles_to_draw) -> void {
  auto [mesh, pos, rot, scale] = render_data;
  // Transform all verts
  math::Matrix4x4 transfmat = CalcTransformMatrix(pos, rot);

  math::Vec3* transformed_verts = new math::Vec3[mesh->verts_count];
  for (int i=0; i < mesh->verts_count; ++i) {
    transformed_verts[i] = mesh->verts[i] * scale * transfmat;
  }

  // Transform all verts to view basis
  math::Vec3* viewed_verts = new math::Vec3[mesh->verts_count];
  for (int i=0; i < mesh->verts_count; ++i) {
    viewed_verts[i] = transformed_verts[i] * viewmat_;
  }
  
  // Filter verts
  for (int i=0; i<mesh->triangles_count; ++i) {
    int verts_id[] = {
      mesh->triangles[i].verts[0],
      mesh->triangles[i].verts[1],
      mesh->triangles[i].verts[2]
    };

    // Skip face if it's normal points at opposite side
    math::Vec3 normal = math::Vec3::Cross(
        viewed_verts[ verts_id[1] ] - viewed_verts[ verts_id[0] ],
        viewed_verts[ verts_id[2] ] - viewed_verts[ verts_id[0] ]);

    if (math::Vec3::Dot(normal, viewed_verts[ verts_id[0] ]) > 0.0f) { continue; }

    // Calc light
    normal = math::Vec3::Cross(
        transformed_verts[ verts_id[1] ] - transformed_verts[ verts_id[0] ],
        transformed_verts[ verts_id[2] ] - transformed_verts[ verts_id[0] ]);
    normal.Normalize();
    float lightk = (1 + math::Vec3::Dot(normal, sun_direction_)) / 2;

    triangles_to_draw.push_back(
      RawTriangle{
        {
          viewed_verts[ verts_id[0] ],
          viewed_verts[ verts_id[1] ],
          viewed_verts[ verts_id[2] ]
        },
        {
          int(mesh->triangles[i].color[0] * lightk),
          int(mesh->triangles[i].color[1] * lightk),
          int(mesh->triangles[i].color[2] * lightk)
        }
      }
    );
  }

  delete [] transformed_verts;
  delete [] viewed_verts;
}

auto Renderer3D::DrawTriangles(
    std::vector<RawTriangle>& triangles_to_draw) -> void {
  for (auto [verts, clr] : triangles_to_draw) {
    // project verts
    for (int i = 0; i < 3; ++i) {
      //int ClippedFaces = 0;
      //face clipped[2];
      //vec3 verts(vertss);
      //ClippedFaces = _face_clip_against_plane(vec3(0.0f, 0.0f, 0.1f), vec3(0.0f, 0.0f, 1.0f), verts[i], clipped[0], clipped[1]);
      //for (int j = 0; j < ClippedFaces; ++j) {
      //verts[i] = clipped[j];
      verts[i] *= projmat_;

      verts[i].x += 1;
      verts[i].y += 1;

      verts[i].x *= 0.5 * context_window_->drawer->resw;
      verts[i].y *= -0.5 * context_window_->drawer->resh;

      verts[i].y += context_window_->drawer->resh;
      //}
    }

    // draw face
    context_window_->drawer->SetDrawColor(clr[0], clr[1], clr[2]);
    context_window_->drawer->DrawTriangle(
      verts[0].x, verts[0].y,
      verts[1].x, verts[1].y,
      verts[2].x, verts[2].y
    );
  }
}


auto FindPlaneIntersectionPoint(
    const math::Vec3 &plane_p,
    const math::Vec3 &plane_n,
    const math::Vec3 &start,
    const math::Vec3 &end) -> math::Vec3 {
  float plane_d = math::Vec3::Dot(plane_n, plane_p);
  float start_d = math::Vec3::Dot(start, plane_n);
  float end_d = math::Vec3::Dot(end, plane_n);
  float t = (plane_d - start_d) / (end_d - start_d);
  math::Vec3 start_to_end = end - start;
  math::Vec3 inter_line = start_to_end * t;
  return start + inter_line;
}

auto FaceClipAgainstPlane(
    const math::Vec3 &plane_p,
    const math::Vec3 &plane_n,
    math::Vec3 in[3],
    math::Vec3 out1[3],
    math::Vec3 out2[3]) -> int {
   math::Vec3* inside[3]; int insideCount = 0;
   math::Vec3* outside[3]; int outsideCount = 0;
 
   float d0 = math::Vec3::Dot(plane_p, in[0]) - math::Vec3::Dot(plane_n, plane_p);
   float d1 = math::Vec3::Dot(plane_p, in[1]) - math::Vec3::Dot(plane_n, plane_p);
   float d2 = math::Vec3::Dot(plane_p, in[2]) - math::Vec3::Dot(plane_n, plane_p);
   
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
     out1[1] = FindPlaneIntersectionPoint(plane_p, plane_n, *inside[0], *outside[0]);
     out1[2] = FindPlaneIntersectionPoint(plane_p, plane_n, *inside[0], *outside[1]);
 
     return 1;
   }
 
   if (insideCount == 2 && outsideCount == 1)
   {
     out1[0] = *inside[0];
     out1[1] = *inside[1];
     out1[2] = FindPlaneIntersectionPoint(plane_p, plane_n, *inside[0], *outside[0]);
 
     out2[0] = *inside[1];
     out2[1] = out1[2];
     out2[2] = FindPlaneIntersectionPoint(plane_p, plane_n, *inside[1], *outside[0]);
 
     return 2;
   }
 
   return 0;
}
}
