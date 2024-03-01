#include "gmpl/components/utils.h"

namespace eng::gmpl {

auto Rotator::SetRotationSpeed(math::Vec3 &new_rotsp) -> void {
  rotsp_ = new_rotsp;
}


auto Rotator::OnUpdate() -> void { 
    entity->transform->Rotate(math::Vec3(0.0f, 0.5f, 0.0f) * scene->delta_time);
}


}
