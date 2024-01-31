#include "gfx/texture-render-data.h"
#include "gfx/triangle.h"

namespace eng::gfx {
  
TextureRenderData::TextureRenderData(
    const math::Vec3& pos,
    const math::Vec3& rot,
    const math::Vec3& scale,
    bool ignore_lightning,
    window::Texture* texture) 
    : RenderData(pos, rot, scale, ignore_lightning), texture_(texture) {
  triangles_ = new Triangle[]{
    Triangle{{0, 1, 2}, {0, 1, 2}, {255, 255, 255}},
    Triangle{{0, 2, 3}, {0, 2, 3}, {255, 255, 255}}
  };

  uv_coords_ = new math::Vec2[] {
    math::Vec2(0.0f, 0.0f),
    math::Vec2(1.0f, 0.0f),
    math::Vec2(1.0f, 1.0f),
    math::Vec2(0.0f, 1.0f)
  };
}


TextureRenderData::~TextureRenderData() {
  delete texture_;
  delete [] triangles_;
  delete [] uv_coords_;
}


// Methods
// ~ Getters
auto TextureRenderData::GetVerts(int* verts_count) const -> math::Vec3* {
  *verts_count = 4;

  int width, height;
  texture_->GetSize(&width, &height);

  math::Vec3* arr = new math::Vec3[]{
    math::Vec3(-width / 2.0f, height / 2.0f, 0.0f),
    math::Vec3(width / 2.0f, height / 2.0f, 0.0f),
    math::Vec3(width / 2.0f, -height / 2.0f, 0.0f),
    math::Vec3(-width / 2.0f, -height / 2.0f, 0.0f),
  };

  return arr;
}


auto TextureRenderData::GetUVCoords(int* uv_coords_count) const -> const math::Vec2* {
  *uv_coords_count = 4;
  return uv_coords_;
}


auto TextureRenderData::GetTriangles(int* triangles_count) const -> const Triangle* {
  *triangles_count = 2;
  return triangles_;
}


auto TextureRenderData::GetTexture() const -> window::Texture* {
  return texture_;
}


}
