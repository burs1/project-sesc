#include "gfx/components/mesh-renderer.h"

namespace eng::gfx {

auto MeshRenderer::SetTexture(window::Texture* new_texture) -> void {
    texture_ = new_texture;
}


auto MeshRenderer::SetMesh(gfx::Mesh* new_mesh) -> void {
    mesh_ = new_mesh;
}


auto MeshRenderer::GetVerts(int* verts_count) const -> math::Vec3* {
  return mesh_->GetVerts(verts_count);
}


auto MeshRenderer::GetUVCoords(int* uv_coords_count) const -> const math::Vec2* {
  return mesh_->GetUVCoords(uv_coords_count);
}


auto MeshRenderer::GetTriangles(int* triangles_count) const -> const Triangle* {
  return mesh_->GetTriangles(triangles_count);
}


auto MeshRenderer::GetTexture() const -> window::Texture* {
  return texture_;
}


}
