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
    const math::Vec3&,
    bool);

  virtual ~RenderData() = default;

  // - Methods -
  // ~ Getters
  auto GetTransform(math::Vec3*, math::Vec3*, math::Vec3*) const -> void;

  // Returns a pointer to copy of array that holds verts.
  // Should be deleted by user.
  virtual auto GetVerts(int*)     const -> math::Vec3* = 0;

  // Returns a pointer to const array that holds UV coords.
  // Shouldn't be deleted by user!
  virtual auto GetUVCoords(int*)  const -> const math::Vec2* = 0;

  // Returns a pointer to const array that holds info about triangles.
  // Shouldn't be deleted by user!
  virtual auto GetTriangles(int*) const -> const Triangle* = 0;

  // Returns a pointer to texture.
  virtual auto GetTexture()       const -> window::Texture* = 0;

  // Returns value of "ignore_lightning_".
  auto IsLightIgnored()           const -> bool;

private:
  math::Vec3 pos_, rot_, scale_;
  bool ignore_lightning_;

};


}
