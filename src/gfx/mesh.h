#pragma once

#include "vec3.h"
#include <vector>
#include <fstream>
#include <strstream>
#include <stdexcept>

namespace engine::gfx {
  struct face {
    int verts[3];
    int color[3] = {255, 255, 255};
  };
  
  class mesh {
  public:
    mesh(const char*);

    ~mesh();

    std::vector<face> faces;
    std::vector<math::vec3> verts;

  private:
    auto _load_from_obj(const char*) -> void;

  };
}
