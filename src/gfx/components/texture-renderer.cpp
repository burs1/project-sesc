#include "gfx/triangle.h"
#include "gfx/components/texture-renderer.h"

namespace eng::gfx {
  
// Methods
auto TextureRenderer::SetTexture(
    const window::Texture* new_texture) -> void {
  texture_ = new_texture;

  if (verts_) { delete verts_; }

  int width, height;
  texture_->GetSize(&width, &height);

  verts_ = new math::Vec3[]{
    math::Vec3(-width / 2.0f, height / 2.0f, 0.0f),
    math::Vec3(width / 2.0f, height / 2.0f, 0.0f),
    math::Vec3(width / 2.0f, -height / 2.0f, 0.0f),
    math::Vec3(-width / 2.0f, -height / 2.0f, 0.0f),
  };
}


auto TextureRenderer::GetVerts(int* verts_count) const -> math::Vec3* {
  *verts_count = 4;
  return verts_;
}


auto TextureRenderer::GetUVCoords(
    int* uv_coords_count) const -> const math::Vec2* {
  *uv_coords_count = 4;
  return uv_coords_;
}


auto TextureRenderer::GetTriangles(
    int* triangles_count) const -> const Triangle* {
  *triangles_count = 2;
  return triangles_;
}


auto TextureRenderer::GetTexture() const -> const window::Texture* {
  return texture_;
}

// - Events -
auto TextureRenderer::OnCreate() -> void {
  uv_coords_ = new math::Vec2[] {
    math::Vec2(0.0f, 0.0f),
    math::Vec2(1.0f, 0.0f),
    math::Vec2(1.0f, 1.0f),
    math::Vec2(0.0f, 1.0f)
  };

  triangles_ = new Triangle[]{
    Triangle{{0, 1, 2}, {0, 1, 2}, {255, 255, 255}},
    Triangle{{0, 2, 3}, {0, 2, 3}, {255, 255, 255}}
  };
}


auto TextureRenderer::OnDestroy() -> void {
  if (verts_) { delete verts_; }
  delete [] uv_coords_;
  delete [] triangles_;
}


}
