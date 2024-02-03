#pragma once

#include "gfx/mesh.h"
#include "gfx/components/renderer.h"
#include "math/vec3.h"
#include "math/vec2.h"

namespace eng::gfx {

class MeshRenderer : public Renderer {
public:
  // - Methods -
  auto SetTexture(window::Texture*) -> void;

  auto SetMesh(gfx::Mesh*)          -> void;

  auto GetVerts(int*)     const -> math::Vec3* override;

  auto GetUVCoords(int*)  const -> const math::Vec2* override;

  auto GetTriangles(int*) const -> const Triangle* override;

  auto GetTexture()       const -> window::Texture* override;


private:
  const Mesh* mesh_ = nullptr;
  window::Texture* texture_ = nullptr;

};

}
