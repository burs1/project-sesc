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
    // === Constructors & Destructors ===
    /// Constructor
    explicit vec3(float p_x = 0, float p_y = 0, float p_z = 0);
    
    /// Copy constructor
    vec3(const vec3&);

    // === Methods ===
    /// vector's length
    auto magnitude()     const -> float;
    
    /// normalize components
    auto normalize()                     -> void;

    /// linear interpolation
    auto lerp(const vec3&, float)       -> void;

    // === Static methods ===
    /// normalize components
    static auto normalized(const vec3&)                       -> vec3;

    /// dot product
    static auto dot(const vec3&, const vec3&)                 -> float;

    /// cross product
    static auto cross(const vec3&, const vec3&)               -> vec3;

    /// linear interpolation
    static auto lerp(const vec3&, const vec3&, float value)  -> vec3;

    // === Overloads ===
    /// Copy 
    auto operator=(const vec3&)          -> void;

    /// Summary
    auto operator+(const vec3&)    const -> vec3;

    auto operator+=(const vec3&)         -> void;

    /// Substract
    auto operator-(const vec3&)    const -> vec3;

    auto operator-=(const vec3&)         -> void;

    /// Scale
    auto operator*(const float&)     const -> vec3;

    auto operator*=(const float&)          -> void;

    auto operator/(const float&)     const -> vec3;

    auto operator/=(const float&)          -> void;

    /// Matrix multiplication
    auto operator*=(const matrix4x4&)       -> void;
    
    auto operator*(const matrix4x4&)        -> vec3;

    /// Components comparison
    auto operator==(const vec3&)   const -> bool;

    auto operator!=(const vec3&)   const -> bool;

    /// Length comparison
    auto operator>(const vec3&)    const -> bool;

    auto operator>=(const vec3&)   const -> bool;

    auto operator<(const vec3&)    const -> bool;

    auto operator<=(const vec3&)   const -> bool;

    // === Attributes ===
    float x, y, z;
  };
}
