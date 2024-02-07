#pragma once

#include "math/vec3.h"
#include "math/vec2.h"
#include "window/texture.h"

namespace eng::gfx {

struct RawTriangle {
  math::Vec3 verts[3];
  math::Vec2 uv_coords[3];
  int color[3];
  const window::Texture* texture;
};

}
