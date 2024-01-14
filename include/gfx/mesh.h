#pragma once

#include <vector>
#include <fstream>
#include <strstream>
#include <stdexcept>

#include "math/vec3.h"
#include "math/vec2.h"

namespace eng::gfx {

struct Triangle {
  int verts[3];
  int uv_coords[3];
  int color[3];
};

struct RawTriangle {
  math::Vec3 verts[3];
  math::Vec2 uv_coords[3];
  int color[3];
  const char* sprite;
};

class Mesh {
public:
  Mesh(const char*, const char* sprite="");

  ~Mesh();

  const char* sprite = "";

  math::Vec3* verts = nullptr;
  math::Vec2* uv_coords = nullptr;
  Triangle* triangles = nullptr;

  unsigned verts_count = 0;
  unsigned uv_coords_count = 0;
  unsigned triangles_count = 0;

  bool is_textured = false;

private:
  auto LoadFromOBJ(const char*) -> void;

  auto ReadOBJ(
    std::ifstream&,
    std::vector<math::Vec3>&,
    std::vector<math::Vec2>&,
    std::vector<Triangle>&) -> void;

  auto CopyDataFromVectors(
    std::vector<math::Vec3>&,
    std::vector<math::Vec2>&,
    std::vector<Triangle>&) -> void;

};

}
