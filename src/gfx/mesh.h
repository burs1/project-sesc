#pragma once

#include "vec3.h"
#include <vector>
#include <fstream>
#include <stdexcept>

namespace engine::gfx {
  struct face {
    int verts[3];
    struct { int r = 255, g = 255, b = 255; } color;
  };
  
  class mesh {
  public:
    mesh(const char*);

    ~mesh();

    std::vector< face >           faces;
    std::vector< math::vec3 >     verts;

  private:
    auto _load_from_obj(const char*) -> void;

  };
}
