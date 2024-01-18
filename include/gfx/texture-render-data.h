#pragma once

#include "gfx/mesh.h"
#include "gfx/render-data.h"
#include "math/vec3.h"
#include "math/vec2.h"

namespace eng::gfx {

class TextureRenderData : public RenderData {
public:
  TextureRenderData(
    const math::Vec3&,
    const math::Vec3&,
    const math::Vec3&,
    window::Texture*);

  ~TextureRenderData();

  // Methods
  // ~ Getters
  auto GetVerts(int*)     const -> math::Vec3* override;

  auto GetUVCoords(int*)  const -> math::Vec2* override;

  auto GetTriangles(int*) const -> const Triangle* override;

  auto GetTexture()       const -> window::Texture* override;

private:
  window::Texture* texture_;
  
  Triangle* triangles_;

};

}
