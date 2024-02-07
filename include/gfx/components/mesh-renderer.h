#pragma once

#include "gfx/mesh.h"
#include "gfx/components/renderer.h"
#include "math/vec3.h"
#include "math/vec2.h"

namespace eng::gfx {

class Scene;

class MeshRenderer : public Renderer {
friend Scene;
 public:
  // Sets renderer texture to a given.
  auto SetTexture(const window::Texture*) -> void;

  // Sets mesh texture to a given.
  auto SetMesh(gfx::Mesh*)          -> void;

  auto GetVerts(int*)     const -> math::Vec3* override;

  auto GetUVCoords(int*)  const -> const math::Vec2* override;

  auto GetTriangles(int*) const -> const Triangle* override;

  auto GetTexture()       const -> const window::Texture* override;

 private:
  const Mesh* mesh_ = nullptr;
  const window::Texture* texture_ = nullptr;
};

}

