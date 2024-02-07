#pragma once

#include "math/vec3.h"
#include "math/matrix4x4.h"
#include "gmpl/component.h"

namespace eng::gmpl {

class Scene;

class Transform : public StaticComponent {
friend Scene;
 public:
  // Sets transform position to a given.
  auto SetPosition(math::Vec3) -> void;

  // Sets transform rotation to a given.
  auto SetRotation(math::Vec3) -> void;

  // Sets transform scale to a give.
  auto SetScale(math::Vec3)    -> void;

  // Returns current transform position.
  auto GetPosition()     const -> math::Vec3;

  // Returns current transform rotation.
  auto GetRotation()     const -> math::Vec3;

  // Returns current transform scale.
  auto GetScale()        const -> math::Vec3;

  // Returns transform matrix.
  auto GetTransformMatrix() -> math::Matrix4x4;

  // Translates transform with a given translation.
  auto Translate(math::Vec3)   -> void;

  // Rotates transform with a given rotation.
  auto Rotate(math::Vec3)      -> void;

  // Rescales transform with a given scaling.
  auto Rescale(math::Vec3)     -> void;

  // Returns world vector that points forward in this transform basis.
  auto Forward() -> math::Vec3;

  // Returns world vector that points right in this transform basis.
  auto Right() -> math::Vec3;

  // Returns world vector that points up in this transform basis.
  auto Up() -> math::Vec3;

private:
  // Updates rotation map if needed.
  auto UpdateRotmat() -> void;

  // Updates transform map if needed.
  auto UpdateTransfmat() -> void;

  math::Vec3 pos_, rot_, scale_ = math::Vec3(1.0f, 1.0f, 1.0f);
  math::Matrix4x4 rotmat_, transfmat_;

  bool is_rotmat_updated_ = true;
  bool is_transfmat_updated_ = true;
};

}

