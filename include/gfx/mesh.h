#pragma once

#include <vector>
#include <fstream>
#include <algorithm>
#include <strstream>
#include <stdexcept>

#include "math/vec3.h"
#include "math/vec2.h"
#include "gfx/triangle.h"

namespace eng::gfx {

class Mesh {
public:
  Mesh(const char*, const char* texture_name="");

  ~Mesh();

  // Methods
  // ~ Getters
  auto GetVerts(int*)     const -> math::Vec3*;

  auto GetUVCoords(int*)  const -> math::Vec2*;

  auto GetTriangles(int*) const -> const Triangle*;

  auto GetTextureName()   const -> const char*;

private:
  // Internal methods
  // ~ OBJ
  auto LoadFromOBJ(const char*) -> void;

  auto ReadOBJ(
    std::ifstream&,
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

  const char* texture_name_ = "";

};

}
