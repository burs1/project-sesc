#pragma once

#include <string>

namespace eng::math {
  class Vec2 {
  public:
    Vec2();

    explicit Vec2(float, float);
    
    Vec2(const Vec2&);

    // methods
    auto Magnitude()        const -> float;
    
    auto Normalize()              -> void;

    auto Lerp(const Vec2&, float) -> void;

    // static methods
    static auto Normalized(const Vec2&)                -> Vec2;

    static auto Dot(const Vec2&, const Vec2&)          -> float;

    static auto Lerp(const Vec2&, const Vec2&, float)  -> Vec2;

    static auto AngleBetween(const Vec2&, const Vec2&) -> float;

    // operators
    auto operator=(const Vec2&)         -> void;

    auto operator+(const Vec2&)   const -> Vec2;

    auto operator+=(const Vec2&)        -> void;

    auto operator-(const Vec2&)   const -> Vec2;

    auto operator-=(const Vec2&)        -> void;

    auto operator*(const float&)  const -> Vec2;

    auto operator*=(const float&)       -> void;

    auto operator/(const float&)  const -> Vec2;

    auto operator/=(const float&)       -> void;

    auto operator*=(const Vec2&)        -> void;

    auto operator*(const Vec2&)   const -> Vec2;

    auto operator==(const Vec2&)  const -> bool;

    auto operator!=(const Vec2&)  const -> bool;

    auto operator>(const Vec2&)   const -> bool;

    auto operator>=(const Vec2&)  const -> bool;

    auto operator<(const Vec2&)   const -> bool;

    auto operator<=(const Vec2&)  const -> bool;

    // vars
    float x, y;
  };
}

namespace std {

string to_string(const eng::math::Vec2&);

}

