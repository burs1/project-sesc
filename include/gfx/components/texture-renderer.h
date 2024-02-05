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
  // - Methods -
  auto SetTexture(window::Texture*) -> void;
  
  auto GetVerts(int*)         const -> math::Vec3* override;

  auto GetUVCoords(int*)      const -> const math::Vec2* override;

  auto GetTriangles(int*)     const -> const Triangle* override;

  auto GetTexture()           const -> window::Texture* override;

private:
  // - Events -
  auto OnCreate()  -> void override;

  auto OnDestroy() -> void override;

  window::Texture* texture_ = nullptr;
  math::Vec3* verts_ = nullptr;
  math::Vec2* uv_coords_ = nullptr;
  Triangle* triangles_ = nullptr;
};

}
