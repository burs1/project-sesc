#pragma once

#include <string>

#include "math/matrix4x4.h"

namespace eng::math {
  class Matrix4x4;
  class Vec3 {
  public:
    Vec3();

    explicit Vec3(float, float, float);
    
    Vec3(const Vec3&);

    // methods
    auto Magnitude()        const -> float;
    
    auto Normalize()              -> void;

    auto Lerp(const Vec3&, float) -> void;

    // static methods
    static auto Normalized(const Vec3&)               -> Vec3;

    static auto Dot(const Vec3&, const Vec3&)         -> float;

    static auto Cross(const Vec3&, const Vec3&)       -> Vec3;

    static auto Lerp(const Vec3&, const Vec3&, float) -> Vec3;

    // operators
    auto operator=(const Vec3&)         -> void;

    auto operator+(const Vec3&)   const -> Vec3;

    auto operator+=(const Vec3&)        -> void;

    auto operator-(const Vec3&)   const -> Vec3;

    auto operator-=(const Vec3&)        -> void;

    auto operator*(const float&)  const -> Vec3;

    auto operator*=(const float&)       -> void;

    auto operator/(const float&)  const -> Vec3;

    auto operator/=(const float&)       -> void;

    auto operator*=(const Matrix4x4&)   -> void;
    
    auto operator*(const Matrix4x4&)    -> Vec3;

    auto operator*=(const Vec3&)        -> void;

    auto operator*(const Vec3&)   const -> Vec3;

    auto operator==(const Vec3&)  const -> bool;

    auto operator!=(const Vec3&)  const -> bool;

    auto operator>(const Vec3&)   const -> bool;

    auto operator>=(const Vec3&)  const -> bool;

    auto operator<(const Vec3&)   const -> bool;

    auto operator<=(const Vec3&)  const -> bool;

    // vars
    float x, y, z;
  };
}

namespace std {

string to_string(const eng::math::Vec3&);

}

