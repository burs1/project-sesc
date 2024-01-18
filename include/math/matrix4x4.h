#pragma once

#include "math/vec3.h"

namespace eng::math {

class Vec3;
class Matrix4x4 {
public:
  Matrix4x4();

  Matrix4x4(const Matrix4x4&);

  // methods
  auto SetIdentity()                   -> void;

  auto SetTranslation(const Vec3&)     -> void;

  auto SetScale(const Vec3&)           -> void;

  auto SetRotation(const Vec3&)        -> void;

  auto SetRotationX(float)              -> void;
  
  auto SetRotationY(float)              -> void;

  auto SetRotationZ(float)              -> void;

  auto SetTransform(const Vec3&, const Vec3&, const Vec3&) -> void;

  auto Right()                    const -> Vec3;

  auto Up()                       const -> Vec3;

  auto Forward()                  const -> Vec3;

  auto SetPerspective(float, float, float, float)                        -> void;

  auto PointAt(const math::Vec3&, const math::Vec3&, const math::Vec3&)  -> void;

  auto LookAt(const math::Vec3&, const math::Vec3&, const math::Vec3&)   -> void;

  auto Inverse() -> void;

  // operators
  auto operator*=(const Matrix4x4&)       -> void;
  
  auto operator*(const Matrix4x4&)  const -> Matrix4x4;

  auto operator=(const Matrix4x4&)        -> void;

  // vars
  float m[4][4];

private:
  // internal methods
  auto _copy(const Matrix4x4&) -> void;
};

}
