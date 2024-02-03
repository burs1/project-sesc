#include "math/matrix4x4.h"
#include "gmpl/entity.h"
#include "gfx/components/renderer.h"

namespace eng::gfx {

auto Renderer::SetIgnoreLightning(bool ignore) -> void {
  ignore_lightning_ = ignore;
}


auto Renderer::IsLightIgnored() const -> bool {
  return ignore_lightning_;
}


auto Renderer::GetTransformMatrix() const -> math::Matrix4x4 {
  return entity->transform->GetTransformMatrix();
}


}
