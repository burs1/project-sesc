#pragma once

#include "gfx/mesh.h"
#include "gfx/render-data.h"
#include "math/vec3.h"
#include "math/vec2.h"

namespace eng::gfx {

class MeshRenderData : public RenderData {
public:
  MeshRenderData(
    const math::Vec3&,
    const math::Vec3&,
    const math::Vec3&,
    const Mesh*,
    window::Texture*);

  auto GetVerts(int*)     const -> math::Vec3* override;

  auto GetUVCoords(int*)  const -> math::Vec2* override;

  auto GetTriangles(int*) const -> const Triangle* override;

  auto GetTexture()       const -> window::Texture* override;

private:
  const Mesh* mesh_ = nullptr;
  window::Texture* texture_ = nullptr;

};

}
