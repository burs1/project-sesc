#pragma once

#include "vec3.h"
#include <vector>
#include <fstream>
#include <stdexcept>

namespace engine::gfx {
  class mesh {
  public:
    mesh(const char*);

    ~mesh();

    struct face { int v1, v2, v3; };

    std::vector< face >           faces;
    std::vector< math::vec3 >     verts;

  private:
    auto _load_from_obj(const char*) -> void;
  };
}
