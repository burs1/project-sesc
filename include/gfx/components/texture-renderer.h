#pragma once

#include "window/texture.h"
#include "gfx/components/renderer.h"
#include "math/vec3.h"
#include "math/vec2.h"

namespace eng::gfx {

class Scene;

class TextureRenderer : public Renderer {
friend Scene;
 public:
  // Sets renderer texture to a given.
  auto SetTexture(const window::Texture*) -> void;
  
  auto GetVerts(int*)         const -> math::Vec3* override;

  auto GetUVCoords(int*)      const -> const math::Vec2* override;

  auto GetNormals(int*)       const -> math::Vec3* override;

  auto GetTriangles(int*)     const -> const Triangle* override;

  auto GetTexture()           const -> const window::Texture* override;

private:
  auto OnCreate()  -> void override;

  auto OnDestroy() -> void override;

  math::Vec3* verts_     = nullptr;
  math::Vec2* uv_coords_ = nullptr;
  math::Vec3* normals_   = nullptr;
  Triangle*   triangles_ = nullptr;
  const window::Texture* texture_ = nullptr;
};

}
