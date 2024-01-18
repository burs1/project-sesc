#include "gfx/render-data.h"

namespace eng::gfx {

RenderData::RenderData(
    const math::Vec3& pos,
    const math::Vec3& rot,
    const math::Vec3& scale)
    : pos_(pos), rot_(rot), scale_(scale) {}

auto RenderData::GetTransform(math::Vec3* pos, math::Vec3* rot, math::Vec3* scale) const -> void {
  *pos = pos_;
  *rot = rot_;
  *scale = scale_;
}

}
