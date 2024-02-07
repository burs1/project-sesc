#pragma once

#include "gfx/triangle.h"
#include "math/vec2.h"
#include "math/vec3.h"
#include "math/matrix4x4.h"
#include "window/texture.h"
#include "gmpl/component.h"

namespace eng::gfx {

class Scene;
class Renderer3D;

class Renderer : public gmpl::StaticComponent {
friend Scene;
friend Renderer3D;
 public:
  // Sets "ignore_lightning_" to a passed value.
  auto SetIgnoreLightning(bool) -> void;

  // Returns value of "ignore_lightning_".
  auto IsLightIgnored()           const -> bool;

 private:
  // Returns a copy of entitiy's transform matrix.
  auto GetTransformMatrix()       const -> math::Matrix4x4;

  // Returns a pointer to an array of mesh verts.
  // (SHOULD BE DELETED BY USER)
  virtual auto GetVerts(int*)     const -> math::Vec3* = 0;

  // Returns a pointer to an array of mesh uv coords.
  // (SHOULDN'T BE DELETED BY USER)
  virtual auto GetUVCoords(int*)  const -> const math::Vec2* = 0;

  // Returns a pointer to an array of mesh triangles.
  // (SHOULDN'T BE DELETED BY USER)
  virtual auto GetTriangles(int*) const -> const Triangle* = 0;

  // Returns a pointer to texture.
  virtual auto GetTexture()       const -> const window::Texture* = 0;

  bool ignore_lightning_;
};

}

