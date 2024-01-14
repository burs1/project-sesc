#include "gfx/renderer3d.h"
#include <stdexcept>

namespace eng::gfx {

Renderer3D* Renderer3D::kInstance = nullptr;

// Static methods
auto Renderer3D::Init() -> void {
  if (not kInstance) {
    kInstance = new Renderer3D();
    return;
  }
  throw std::runtime_error("Renderer3D is already online.");
}

auto Renderer3D::GetInstance() -> Renderer3D* {
  if (kInstance) {
    return kInstance;
  }
  throw std::runtime_error("Renderer3D is offline.");
}

auto Renderer3D::Quit() -> void {
  if (kInstance) {
    delete kInstance;
    return;
  }
  throw std::runtime_error("Renderer3D is already offline.");
}

Renderer3D::Renderer3D()
  : context_window_(sdl::Window::GetInstance()) {
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

auto Renderer3D::SetCameraTransform(const math::Vec3* pos,
                                    const math::Vec3* rot) -> void {
  camera_ = {pos, rot};
}


auto Renderer3D::SetSunDirection(const math::Vec3 &dir) -> void {
  sun_direction_ = dir;
  sun_direction_.Normalize();
}


// ~ resources
auto Renderer3D::LoadMesh(const char* path, const char* name, const char* sprite) -> void {
  // Check if mesh with such name already exists
  if (meshes_.contains(name)) { 
    throw std::runtime_error("Mesh with name \"" + std::string(name) + "\" already exists");
  }

  meshes_[name] = new Mesh(path, sprite);
}


// ~ draw
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
  rotmat.SetRotation(*camera_.rot);
  
  viewmat_.LookAt(*camera_.pos, *camera_.pos + rotmat.Forward(), rotmat.Up());
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
          mesh->uv_coords[ mesh->triangles[i].uv_coords[0] ],
          mesh->uv_coords[ mesh->triangles[i].uv_coords[1] ],
          mesh->uv_coords[ mesh->triangles[i].uv_coords[2] ]
        },
        {
          int(mesh->triangles[i].color[0] * lightk),
          int(mesh->triangles[i].color[1] * lightk),
          int(mesh->triangles[i].color[2] * lightk)
        },
        mesh->sprite
      }
    );
  }

  delete [] transformed_verts;
  delete [] viewed_verts;
}

auto Renderer3D::DrawTriangles(
    std::vector<RawTriangle>& triangles_to_draw) -> void {
  for (auto& triangle : triangles_to_draw) {
    // Clip triangle
    int clipped_triangles_count = 0;
    RawTriangle clipped_tris[2];
    clipped_triangles_count = TriangleClipAgainstPlane(
      math::Vec3(0.0f, 0.0f, 0.1f),
      math::Vec3(0.0f, 0.0f, 1.0f),
      triangle,
      clipped_tris[0], clipped_tris[1]);
 
    for (int i=0; i<clipped_triangles_count; ++i) {
      ProjectTriangle(clipped_tris[i]);
      DrawTriangle(clipped_tris[i]);
    }
  }
}

auto Renderer3D::DrawTriangle(RawTriangle& triangle) -> void {
  context_window_->drawer->SetDrawColor(
    triangle.color[0],
    triangle.color[1],
    triangle.color[2]);

  if (triangle.sprite[0] == '\0') {
    context_window_->drawer->DrawTriangle(
      triangle.verts[0].x, triangle.verts[0].y,
      triangle.verts[1].x, triangle.verts[1].y,
      triangle.verts[2].x, triangle.verts[2].y
    );
    return;
  }

  context_window_->drawer->DrawTriangleTextured(
    triangle.verts[0].x, triangle.verts[0].y,
    triangle.verts[1].x, triangle.verts[1].y,
    triangle.verts[2].x, triangle.verts[2].y,
    triangle.uv_coords[0].x, triangle.uv_coords[0].y, 
    triangle.uv_coords[1].x, triangle.uv_coords[1].y,
    triangle.uv_coords[2].x, triangle.uv_coords[2].y,
    triangle.sprite
  );
}


auto Renderer3D::ProjectTriangle(RawTriangle& triangle) -> void {
  for (int j=0; j < 3; ++j) {
    triangle.verts[j] *= projmat_;

    triangle.verts[j].x += 1;
    triangle.verts[j].y += 1;

    triangle.verts[j].x *= 0.5 * context_window_->drawer->resw;
    triangle.verts[j].y *= -0.5 * context_window_->drawer->resh;

    triangle.verts[j].y += context_window_->drawer->resh;
  }
}


auto Renderer3D::FindPlaneIntersectionPoint(
    math::Vec3 plane_p,
    math::Vec3 plane_n,
    const math::Vec3 &start,
    const math::Vec3 &end,
    float &t) -> math::Vec3 {
  plane_n.Normalize();

  float plane_d = -math::Vec3::Dot(plane_n, plane_p);
  float start_d = math::Vec3::Dot(start, plane_n);
  float end_d = math::Vec3::Dot(end, plane_n);
  t = (-plane_d - start_d) / (end_d - start_d);
  
  math::Vec3 start_to_end = end - start;
  math::Vec3 inter_line = start_to_end * t;
  
  return start + inter_line;
}

auto Renderer3D::TriangleClipAgainstPlane(
    math::Vec3 plane_p,
    math::Vec3 plane_n,
    RawTriangle in,
    RawTriangle& out1,
    RawTriangle& out2) -> int {
  plane_n.Normalize();

  auto dist = [&](math::Vec3 &p) {
    math::Vec3 n = math::Vec3::Normalized(p);
    return math::Vec3::Dot(plane_n, p) - math::Vec3::Dot(plane_n, plane_p);
  };

  math::Vec3* inside[3]; int inside_count = 0; 
  math::Vec3* outside[3]; int outside_count = 0;

  math::Vec2* inside_uv[3]; int inside_uv_count = 0;
  math::Vec2* outside_uv[3]; int outside_uv_count = 0;
 
  float d[3];
  d[0] = dist(in.verts[0]);
  d[1] = dist(in.verts[1]);
  d[2] = dist(in.verts[2]);
   
  for (int i=0; i<3; ++i) {
    if (d[i] >= 0) {
     inside[inside_count++] = &in.verts[i];
     inside_uv[inside_uv_count] = &in.uv_coords[i];
    }
    else {
     outside[outside_count++] = &in.verts[i];
     outside_uv[outside_uv_count++] = &in.uv_coords[i];
    }
  }
 
   if (inside_count == 0) {
     return 0;
   }
 
   if (inside_count == 3) {
     out1 = in;
     return 1;
   }
 
   if (inside_count == 1 && outside_count == 2) {
     float t;

     out1.sprite = in.sprite;
     out1.color[0] = in.color[0];
     out1.color[1] = in.color[1];
     out1.color[2] = in.color[2];

     out1.verts[0] = *inside[0];
     out1.uv_coords[0] = *inside_uv[0];

     out1.verts[1] = FindPlaneIntersectionPoint(plane_p, plane_n, *inside[0], *outside[0], t);
     out1.uv_coords[1].x = t * (outside_uv[0]->x - inside_uv[0]->x) + inside_uv[0]->x;
     out1.uv_coords[1].y = t * (outside_uv[0]->y - inside_uv[0]->y) + inside_uv[0]->y;

     out1.verts[2] = FindPlaneIntersectionPoint(plane_p, plane_n, *inside[0], *outside[1], t);
     out1.uv_coords[2].x = t * (outside_uv[1]->x - inside_uv[1]->x) + inside_uv[1]->x;
     out1.uv_coords[2].y = t * (outside_uv[1]->y - inside_uv[1]->y) + inside_uv[1]->y;

     return 1;
   }
 
   if (inside_count == 2 && outside_count == 1) {
     float t;

     out1.sprite = in.sprite;
     out1.color[0] = in.color[0];
     out1.color[1] = in.color[1];
     out1.color[2] = in.color[2];

     out1.verts[0] = *inside[0];
     out1.uv_coords[0] = *inside_uv[0];

     out1.verts[1] = *inside[1];
     out1.uv_coords[1] = *inside_uv[1];

     out1.verts[2] = FindPlaneIntersectionPoint(plane_p, plane_n, *inside[0], *outside[0], t);
     out1.uv_coords[2].x = t * (outside_uv[0]->x - inside_uv[0]->x) + inside_uv[0]->x;
     out1.uv_coords[2].y = t * (outside_uv[0]->y - inside_uv[0]->y) + inside_uv[0]->y;
 
     out2.sprite = in.sprite;
     out2.color[0] = in.color[0];
     out2.color[1] = in.color[1];
     out2.color[2] = in.color[2];

     out2.verts[0] = *inside[1];
     out2.uv_coords[0] = *inside_uv[1];

     out2.verts[1] = out1.verts[2];
     out2.uv_coords[1] = out1.uv_coords[2];

     out2.verts[2] = FindPlaneIntersectionPoint(plane_p, plane_n, *inside[1], *outside[0], t);
     out2.uv_coords[2].x = t * (outside_uv[0]->x - inside_uv[1]->x) + inside_uv[1]->x;
     out2.uv_coords[2].y = t * (outside_uv[0]->y - inside_uv[1]->y) + inside_uv[1]->y;
 
     return 2;
   }

   return 0;
}

}
