/*
 * Author: Ilya Buravov
 *
 * Summary: 3D vector implementation
 *
 */

#pragma once

#include <cmath>
#include "matrix4x4.h"

namespace engine::math {
  class matrix4x4;
  class vec3 {
  public:
    explicit vec3(float p_x = 0, float p_y = 0, float p_z = 0);
    
    vec3(const vec3&);

    // methods
    auto magnitude()        const -> float;
    
    auto normalize()              -> void;

    auto lerp(const vec3&, float) -> void;

    // static methods
    static auto normalized(const vec3&)                       -> vec3;

    static auto dot(const vec3&, const vec3&)                 -> float;

    static auto cross(const vec3&, const vec3&)               -> vec3;

    static auto lerp(const vec3&, const vec3&, float value)   -> vec3;

    // operators
    auto operator=(const vec3&)          -> void;

    auto operator+(const vec3&)    const -> vec3;

    auto operator+=(const vec3&)         -> void;

    auto operator-(const vec3&)    const -> vec3;

    auto operator-=(const vec3&)         -> void;

    auto operator*(const float&)     const -> vec3;

    auto operator*=(const float&)          -> void;

    auto operator/(const float&)     const -> vec3;

    auto operator/=(const float&)          -> void;

    auto operator*=(const matrix4x4&)       -> void;
    
    auto operator*(const matrix4x4&)        -> vec3;

    auto operator==(const vec3&)   const -> bool;

    auto operator!=(const vec3&)   const -> bool;

    auto operator>(const vec3&)    const -> bool;

    auto operator>=(const vec3&)   const -> bool;

    auto operator<(const vec3&)    const -> bool;

    auto operator<=(const vec3&)   const -> bool;

    // vars
    float x, y, z;
  };
}
