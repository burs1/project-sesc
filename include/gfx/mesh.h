#pragma once

#include <vector>
#include <fstream>
#include <strstream>
#include <stdexcept>

#include "math/vec3.h"

namespace eng::gfx {
  struct Triangle {
    int verts[3];
    int color[3];
  };
  
  struct RawTriangle {
    math::Vec3 verts[3];
    int color[3];
  };

  
  class Mesh {
  public:
    Mesh(const char*);

    ~Mesh();

    math::Vec3* verts = nullptr;
    Triangle* triangles = nullptr;

    unsigned verts_count = 0;
    unsigned triangles_count = 0;

  private:
    auto LoadFromOBJ(const char*) -> void;

    auto ReadOBJ(
      std::ifstream&,
      std::vector<math::Vec3>&,
      std::vector<Triangle>&) -> void;

    auto CopyDataFromVectors(
      std::vector<math::Vec3>&,
      std::vector<Triangle>&) -> void;

  };
}
