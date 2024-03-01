#pragma once

#include "math/vec3.h"

namespace eng::gfx {

struct Triangle {
  int verts_ids[3];
  int uv_coords_ids[3];
  int normal_id;
  int color[3];
};

}
