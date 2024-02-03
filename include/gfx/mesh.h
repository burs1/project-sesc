#pragma once

#include <vector>

#include "math/vec3.h"
#include "math/vec2.h"
#include "gfx/triangle.h"

namespace eng::gfx {

class Renderer3D;

class Mesh {
friend Renderer3D;
public:
  // Methods
  auto GetVerts(int*)     const -> math::Vec3*;

  auto GetUVCoords(int*)  const -> const math::Vec2*;

  auto GetTriangles(int*) const -> const Triangle*;

private:
  // Constructor
  explicit Mesh(const char*);

  ~Mesh();

  // Internal methods
  auto LoadFromOBJ(const char*) -> void;

  auto ReadOBJ(
    const char*,
    std::vector<math::Vec3>&,
    std::vector<math::Vec2>&,
    std::vector<Triangle>&) -> void;

  // ~ Copy
  auto CopyDataFromVectors(
    std::vector<math::Vec3>&,
    std::vector<math::Vec2>&,
    std::vector<Triangle>&) -> void;

  // Vars
  math::Vec3* verts_ = nullptr;
  int verts_count_ = 0;

  math::Vec2* uv_coords_ = nullptr;
  int uv_coords_count_ = 0;

  Triangle* triangles_ = nullptr;
  int triangles_count_ = 0;

};

}
