/*
 * Author: Ilya Buravov
 *
 * Summary: 2D vector implementation
 *
 */

#pragma once

#include <cmath>

namespace engine::math {
  class vec2 {
  public:
    explicit vec2(double p_x = 0, double p_y = 0);
    
    vec2(const vec2&);

    // methods
    auto magnitude() const -> double;

    auto normalize()       -> void;

    auto lerp(const vec2&, double value)                      -> void;

    // static methods
    static auto normalized(const vec2&)                       -> vec2;

    static auto dot(const vec2&, const vec2&)                 -> double;

    static auto lerp(const vec2&, const vec2&, double value)  -> vec2;

    // operatos
    auto operator=(const vec2&)          -> void;

    auto operator+(const vec2&)  const   -> vec2;

    auto operator+=(const vec2&)         -> void;

    auto operator-(const vec2&)  const   -> vec2;

    auto operator-=(const vec2&)         -> void;

    auto operator*(const double&)   const   -> vec2;
    
    auto operator*=(const double &)         -> void;
    
    auto operator/(const double&)   const   -> vec2;

    auto operator/=(const double&)          -> void;
  
    auto operator*(const vec2&)  const   -> double;

    auto operator==(const vec2&) const   -> bool;

    auto operator!=(const vec2&) const   -> bool;
    
    auto operator>(const vec2&)  const   -> bool;

    auto operator>=(const vec2&) const   -> bool;

    auto operator<(const vec2&)  const   -> bool;

    auto operator<=(const vec2&) const   -> bool;

    // vars
    double x, y;
  };
}
