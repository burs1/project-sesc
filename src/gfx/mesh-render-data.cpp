#include "gfx/mesh-render-data.h"
#include "window/texture.h"

namespace eng::gfx {

MeshRenderData::MeshRenderData(
    const math::Vec3& pos,
    const math::Vec3& rot,
    const math::Vec3& scale,
    const Mesh* mesh,
    window::Texture* texture)
    : RenderData(pos, rot, scale), mesh_(mesh) {}


auto MeshRenderData::GetVerts(int* verts_count) const -> math::Vec3* {
  return mesh_->GetVerts(verts_count);
}


auto MeshRenderData::GetUVCoords(int* uv_coords_count) const -> const math::Vec2* {
  return mesh_->GetUVCoords(uv_coords_count);
}


auto MeshRenderData::GetTriangles(int* triangles_count) const -> const Triangle* {
  return mesh_->GetTriangles(triangles_count);
}


auto MeshRenderData::GetTexture() const -> window::Texture* {
  return texture_;
}


}
