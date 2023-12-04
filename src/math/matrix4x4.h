/*
 * Author: Ilya Buravov
 * 
 * Summary: Simple 4x4 matrix implementation
 *
 */

#pragma once

#include "vec3.h"

namespace engine::math {
  class vec3;
  class matrix4x4 {
  public:
    matrix4x4();

    matrix4x4(const matrix4x4&);

    // methods
    auto set_identity()                   -> void;

    auto set_translation(const vec3&)     -> void;

    auto get_translation()          const -> vec3;

    auto right()                    const -> vec3;

    auto up()                       const -> vec3;

    auto forward()                  const -> vec3;

    auto set_scale(const vec3&)           -> void;

    auto set_rot(const vec3&)             -> void;

    auto set_rotX(float)                  -> void;
    
    auto set_rotY(float)                  -> void;

    auto set_rotZ(float)                  -> void;

    auto set_perspective(float, float, float, float)                        -> void;

    auto point_at(const math::vec3&, const math::vec3&, const math::vec3&)  -> void;

    auto look_at(const math::vec3&, const math::vec3&, const math::vec3&)   -> void;

    auto inverse() -> void;

    // operators
    auto operator*=(const matrix4x4&)       -> void;
    
    auto operator*(const matrix4x4&)  const -> matrix4x4;

    auto operator=(const matrix4x4&)        -> void;

    // vars
    float m[4][4];

  private:
    // internal methods
    auto _copy(const matrix4x4&) -> void;
  };
}
