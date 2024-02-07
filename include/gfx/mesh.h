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
  // Returns a pointer to an array of mesh verts.
  // (SHOULD BE DELETED BY USER)
  auto GetVerts(int*)     const -> math::Vec3*;

  // Returns a pointer to an array of mesh uv coords.
  // (SHOULDN'T BE DELETED BY USER)
  auto GetUVCoords(int*)  const -> const math::Vec2*;

  // Returns a pointer to an array of mesh triangles.
  // (SHOULDN'T BE DELETED BY USER)
  auto GetTriangles(int*) const -> const Triangle*;

 private:
  explicit Mesh(const char*);
  ~Mesh();

  // Loads data from given obj file.
  auto LoadFromOBJ(const char*) -> void;

  // Parses OBJ file data.
  auto ReadOBJ(
    const char*,
    std::vector<math::Vec3>&,
    std::vector<math::Vec2>&,
    std::vector<Triangle>&) -> void;

  // Copies data from vectors to arrays.
  auto CopyDataFromVectors(
    std::vector<math::Vec3>&,
    std::vector<math::Vec2>&,
    std::vector<Triangle>&) -> void;

  math::Vec3* verts_ = nullptr;
  int verts_count_ = 0;

  math::Vec2* uv_coords_ = nullptr;
  int uv_coords_count_ = 0;

  Triangle* triangles_ = nullptr;
  int triangles_count_ = 0;
};

}

