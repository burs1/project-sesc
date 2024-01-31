#include <map>
#include <string>
#include <vector>
#include <stdexcept>
#include <algorithm>

#include "gfx/mesh.h"
#include "gfx/renderer3d.h"
#include "gfx/render-data.h"
#include "gfx/raw-triangle.h"
#include "gfx/mesh-render-data.h"
#include "gfx/texture-render-data.h"
#include "math/vec3.h"
#include "math/matrix4x4.h"
#include "logger/logger.h"

namespace eng::gfx {

Renderer3D* Renderer3D::kInstance = nullptr;

// Static methods
auto Renderer3D::Create() -> void {
  if (not kInstance) {
    kInstance = new Renderer3D();
    log::Info("Renderer3D instance created");
    return;
  }

  throw std::runtime_error("Trying to create a renderer3D instance when it is already open");
}

auto Renderer3D::GetInstance() -> Renderer3D* {
  if (kInstance) { return kInstance; }

  throw std::runtime_error("Trying to get a renderer3D instance when it doesn't exist");
}

auto Renderer3D::Destroy() -> void {
  if (kInstance) {
    log::Info("Renderer3D instance destroyed");
    delete kInstance;
    return;
  }

  throw std::runtime_error("Trying to destroy a renderer3D instance when it doens't exist");
}

Renderer3D::Renderer3D()
  : drawer_(window::Drawer::GetInstance()) {
    SetPerspective(70.0f, 0.1f, 1000.0f);
    SetSunRotation(math::Vec3(1, 1, 1));
}

Renderer3D::~Renderer3D() {
  // Destroy all unredered render data
  for (auto render_data : render_queue_) {
    delete render_data;
  }

  // Unload all loaded meshes
  for (auto [name, mesh] : meshes_) {
    delete mesh;
  }

  // Maps and vectors frees automaticly
}


// Methods
// ~ Resources
auto Renderer3D::LoadMesh(
    const char* path, const char* mesh_name, const char* sprite) -> void {
  // Check if mesh with such name already exists
  if (meshes_.contains(mesh_name)) {
    throw std::runtime_error("Mesh with name \"" + std::string(mesh_name) + "\" already exists");
  }

  meshes_[mesh_name] = new Mesh(path, sprite);
}


auto Renderer3D::UnloadMesh(const char* mesh_name) -> void {
  delete meshes_[mesh_name];
  meshes_.erase(mesh_name);
}


// ~ Setters
auto Renderer3D::SetPerspective(float fov, float near, float far) -> void {
  float aspect_ratio = drawer_->GetAspectRatio();
  projmat_.SetPerspective(fov, aspect_ratio, near, far);

  fov_ = fov;
  near_ = near;
  far_ = far;
}


auto Renderer3D::SetFOV(float fov) -> void {
  float aspect_ratio = drawer_->GetAspectRatio();
  projmat_.SetPerspective(fov, aspect_ratio, near_, far_);

  fov_ = fov;
}


auto Renderer3D::SetNearPlane(float near) -> void {
  float aspect_ratio = drawer_->GetAspectRatio();
  projmat_.SetPerspective(fov_, aspect_ratio, near, far_);

  near_ = near;
}


auto Renderer3D::SetFarPlane(float far) -> void {
  float aspect_ratio = drawer_->GetAspectRatio();
  projmat_.SetPerspective(fov_, aspect_ratio, near_, far);

  far_ = far;
}


auto Renderer3D::SetPlanes(float far, float near) -> void {
  float aspect_ratio = drawer_->GetAspectRatio();
  projmat_.SetPerspective(fov_, aspect_ratio, near, far);

  far_ = far;
  near_ = near;
}


auto Renderer3D::SetCameraTransform(
    const math::Vec3* pos,
    const math::Vec3* rot) -> void {
  camera_ = {pos, rot};
}


auto Renderer3D::SetSunRotation(const math::Vec3 &dir) -> void {
  sun_direction_ = dir;
  sun_direction_.Normalize();
}


// ~ Render
auto Renderer3D::AddMeshToQueue(
    const math::Vec3& pos,
    const math::Vec3& rot,
    const math::Vec3& scale,
    bool ignore_lightning,
    const char* mesh_name) -> void {
  Mesh* mesh = meshes_[mesh_name];
  render_queue_.push_back(
    new MeshRenderData(
      pos, rot, scale, ignore_lightning, mesh,
      drawer_->GetTexture( mesh->GetTextureName() )
    ));
}


auto Renderer3D::AddTextureToQueue(
    const math::Vec3& pos,
    const math::Vec3& rot,
    const math::Vec3& scale,
    bool ignore_lightning,
    const char* texture_name) -> void {
  render_queue_.push_back(
    new TextureRenderData(
      pos, rot, scale, ignore_lightning,
      drawer_->GetTexture(texture_name)
    ));
}


auto Renderer3D::AddTextToQueue(
    const math::Vec3& pos,
    const math::Vec3& rot,
    const math::Vec3& scale,
    bool ignore_lightning,
    const char* text) -> void {

  window::Texture* text_texture;
  render_queue_.push_back(
    new TextureRenderData(
      pos, rot, scale, ignore_lightning,
      drawer_->RenderText(text)
    ));
}


auto Renderer3D::RenderQueue() -> void {
  // Calc view matrix
  log::Info("Renderer3D: start frame rendering ");
  CalcViewMatrix();
  std::vector<RawTriangle> triangles_to_draw;

  // Render Queue
  log::Info("Renderer3D: process " + std::to_string(render_queue_.size()) + " render data object(s)");
  for(auto& render_data : render_queue_) {
    ProcessTriangles(render_data, triangles_to_draw);
  }

  SortTriangles(triangles_to_draw);

  DrawTriangles(triangles_to_draw);

  log::Info("Renderer3D: free up memory");
  // Destroy all render data
  for (auto render_data : render_queue_) {
    delete render_data;
  }
  render_queue_.clear();
}

// Internal methods
// ~ Render
auto Renderer3D::CalcViewMatrix() -> void {
  math::Matrix4x4 rotmat;

  viewmat_.LookAt(math::Vec3(0, 0, 0), math::Vec3(0, 0, 1), math::Vec3(0, 1, 0));

  if (not camera_.rot) {
    log::Warn("Renderer3D: using default cam rotation");
    math::Vec3 default_rot(0.0f, 0.0f, 0.0f);
    rotmat.SetRotation(default_rot);
  }
  else { rotmat.SetRotation(*camera_.rot); }

  if (not camera_.pos) {
    log::Warn("Renderer3D: using default cam position");
    math::Vec3 default_pos(0.0f, 0.0f, 0.0f);
    viewmat_.LookAt(
      default_pos,
      default_pos + rotmat.Forward(),
      rotmat.Up());
  }
  else {
    viewmat_.LookAt(
      *camera_.pos,
      *camera_.pos + rotmat.Forward(),
      rotmat.Up());
  }
}


auto Renderer3D::CalcTransformMatrix(
    const math::Vec3& pos,
    const math::Vec3& rot,
    const math::Vec3& scale) -> math::Matrix4x4 {
  math::Matrix4x4 transf_mat;
  transf_mat.SetTransform(pos, rot, scale);
  return transf_mat;
}


auto Renderer3D::ProcessTriangles(
    const RenderData* render_data,
    std::vector<RawTriangle>& triangles_to_draw) -> void {
  // Calc transform matrix
  math::Vec3 pos, rot, scale;
  render_data->GetTransform(&pos, &rot, &scale);
  math::Matrix4x4 transfmat = CalcTransformMatrix(pos, rot, scale);

  // Copy and transform verts
  int verts_count;
  math::Vec3* transformed_verts = render_data->GetVerts(&verts_count);
  for (int i=0; i<verts_count; ++i) {
    transformed_verts[i] *= transfmat;
  }

  // Copy and transform all verts to view basis
  math::Vec3* viewed_verts = new math::Vec3[verts_count];
  for (int i=0; i < verts_count; ++i) {
    viewed_verts[i] = transformed_verts[i] * viewmat_;
  }

  // Copy uv coords (POINTER SHOULDN'T BE DELETED)
  int uv_coords_count;
  const math::Vec2* uv_coords = render_data->GetUVCoords(&uv_coords_count);
  
  // Copy triangles (POINTER SHOULDN'T BE DELETED)
  int triangles_count;
  const Triangle* triangles = render_data->GetTriangles(&triangles_count);

  // Filter verts
  for (int i=0; i<triangles_count; ++i) {
    int verts_ids[] = {
      triangles[i].verts_ids[0],
      triangles[i].verts_ids[1],
      triangles[i].verts_ids[2]
    };

    // Skip face if it's normal points at opposite side
    math::Vec3 normal = math::Vec3::Cross(
        viewed_verts[ verts_ids[1] ] - viewed_verts[ verts_ids[0] ],
        viewed_verts[ verts_ids[2] ] - viewed_verts[ verts_ids[0] ]);

    if (math::Vec3::Dot(normal, viewed_verts[ verts_ids[0] ]) > 0.0f) {
      continue;
    }

    // Calc light
    float lightk = 1.0f;
    if (not render_data->IsLightIgnored()) {
      normal = math::Vec3::Cross(
          transformed_verts[ verts_ids[1] ] - transformed_verts[ verts_ids[0] ],
          transformed_verts[ verts_ids[2] ] - transformed_verts[ verts_ids[0] ]);
      normal.Normalize();
      lightk = (1 + math::Vec3::Dot(normal, sun_direction_)) / 2;
    }

    triangles_to_draw.push_back(
      RawTriangle{
        {
          viewed_verts[ verts_ids[0] ],
          viewed_verts[ verts_ids[1] ],
          viewed_verts[ verts_ids[2] ]
        },
        {
          uv_coords[ triangles[i].uv_coords_ids[0] ],
          uv_coords[ triangles[i].uv_coords_ids[1] ],
          uv_coords[ triangles[i].uv_coords_ids[2] ]
        },
        {
          int(triangles[i].color[0] * lightk),
          int(triangles[i].color[1] * lightk),
          int(triangles[i].color[2] * lightk)
        },
        render_data->GetTexture()
      }
    );
  }

  // Clean up
  delete [] transformed_verts;
  delete [] viewed_verts;
}

auto Renderer3D::SortTriangles(std::vector<RawTriangle>& triangles) -> void {
  log::Info("Renderer3D: sort " + std::to_string(triangles.size()) +  " triangle(s)");
  sort(
    triangles.begin(),
    triangles.end(),
    [](RawTriangle &trng1, RawTriangle &trng2) {
      float z1 = trng1.verts[0].z + trng1.verts[1].z + trng1.verts[2].z;
      float z2 = trng2.verts[0].z + trng2.verts[1].z + trng2.verts[2].z;
      return z1 > z2;
    }
  );
}

auto Renderer3D::DrawTriangles(
    std::vector<RawTriangle>& triangles_to_draw) -> void {
  log::Info("Renderer3D: draw " + std::to_string(triangles_to_draw.size()) + " triangle(s)");
  for (auto& triangle : triangles_to_draw) {
    // Clip triangle
    int clipped_triangles_count = 0;
    RawTriangle clipped_tris[2];
    clipped_triangles_count = ClipTriangleAgainstPlane(
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
  drawer_->SetDrawColor(
    triangle.color[0],
    triangle.color[1],
    triangle.color[2]);

  if (not triangle.texture) {
    drawer_->DrawTriangle(
      triangle.verts[0].x, triangle.verts[0].y,
      triangle.verts[1].x, triangle.verts[1].y,
      triangle.verts[2].x, triangle.verts[2].y
    );
    return;
  }

  drawer_->DrawTriangleTextured(
    triangle.verts[0].x, triangle.verts[0].y,
    triangle.verts[1].x, triangle.verts[1].y,
    triangle.verts[2].x, triangle.verts[2].y,
    triangle.uv_coords[0].x, triangle.uv_coords[0].y, 
    triangle.uv_coords[1].x, triangle.uv_coords[1].y,
    triangle.uv_coords[2].x, triangle.uv_coords[2].y,
    triangle.texture
  );
}


auto Renderer3D::ProjectTriangle(RawTriangle& triangle) -> void {
  int width, height;
  drawer_->GetResolution(&width, &height);
  
  for (int j=0; j < 3; ++j) {
    triangle.verts[j] *= projmat_;

    triangle.verts[j].x += 1;
    triangle.verts[j].y += 1;

    triangle.verts[j].x *= 0.5 * width;
    triangle.verts[j].y *= -0.5 * height;

    triangle.verts[j].y += height;
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

auto Renderer3D::ClipTriangleAgainstPlane(
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
 
   if (inside_count == 0) { return 0; }
 
   if (inside_count == 3) {
     out1 = in;
     return 1;
   }
 
   if (inside_count == 1 && outside_count == 2) {
     float t;

     out1.texture = in.texture;
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

     // Copy texture and color
     out1 = in;

     out1.verts[0] = *inside[0];
     out1.uv_coords[0] = *inside_uv[0];

     out1.verts[1] = *inside[1];
     out1.uv_coords[1] = *inside_uv[1];

     out1.verts[2] = FindPlaneIntersectionPoint(plane_p, plane_n, *inside[0], *outside[0], t);
     out1.uv_coords[2].x = t * (outside_uv[0]->x - inside_uv[0]->x) + inside_uv[0]->x;
     out1.uv_coords[2].y = t * (outside_uv[0]->y - inside_uv[0]->y) + inside_uv[0]->y;
 
     // Copy texture and color
     out2 = in;

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
