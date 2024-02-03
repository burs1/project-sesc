#pragma once

#include "gfx/triangle.h"
#include "math/vec2.h"
#include "math/vec3.h"
#include "math/matrix4x4.h"
#include "window/texture.h"
#include "gmpl/component.h"

namespace eng::gfx {

class Renderer3D;

class Renderer : gmpl::StaticComponent {
friend Renderer3D;
public:
  // Sets "ignore_lightning_" to a passed value.
  auto SetIgnoreLightning(bool) -> void;


  // Returns value of "ignore_lightning_".
  auto IsLightIgnored()           const -> bool;


private:
  // - Methods -
  // Returns a copy of entitiy's transform matrix.
  auto GetTransformMatrix()       const -> math::Matrix4x4;


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


private:
  bool ignore_lightning_;

};


}
