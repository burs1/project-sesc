#pragma once

#include "gfx/triangle.h"
#include "math/vec3.h"
#include "math/vec2.h"
#include "window/texture.h"

namespace eng::gfx {

class RenderData {
public:
  RenderData(
    const math::Vec3&,
    const math::Vec3&,
    const math::Vec3&);

  virtual ~RenderData() = 0;

  // Methods
  // ~ Getters
  auto GetTransform(math::Vec3*, math::Vec3*, math::Vec3*) const -> void;

  virtual auto GetVerts(int*)     const -> math::Vec3* = 0;

  virtual auto GetUVCoords(int*)  const -> math::Vec2* = 0;

  virtual auto GetTriangles(int*) const -> const Triangle* = 0;

  virtual auto GetTexture()       const -> window::Texture* = 0;

private:
  math::Vec3 pos_, rot_, scale_;

};


}
