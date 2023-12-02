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
    // === Constructors & Destructors ===
    /// Constructor
    explicit vec2(double p_x = 0, double p_y = 0);
    
    /// Copy Constructor
    vec2(const vec2&);

    // === Methods ===
    /// Vector's length
    constexpr auto magnitude()      const -> double;

    /// Normalize components
    auto normalize()                      -> void;

    /// Linear interpolation
    auto lerp(const vec2&, double value)  -> void;

    // === Static methods ===
    static auto normalized(const vec2&)                       -> vec2;

    static constexpr auto dot(const vec2&, const vec2&)       -> double;

    static auto lerp(const vec2&, const vec2&, double value)  -> vec2;

    // === Overloads ===
    /// Copy
    auto operator=(const vec2&)          -> void;

    /// Summary
    auto operator+(const vec2&)  const   -> vec2;

    auto operator+=(const vec2&)         -> void;

    /// Substract
    auto operator-(const vec2&)  const   -> vec2;

    auto operator-=(const vec2&)         -> void;

    /// Scale
    auto operator*(const double&)   const   -> vec2;
    
    auto operator*=(const double &)         -> void;
    
    auto operator/(const double&)   const   -> vec2;

    auto operator/=(const double&)          -> void;
  
    /// Dot product
    auto operator*(const vec2&)  const   -> double;

    /// Components comparison
    auto operator==(const vec2&) const   -> bool;

    auto operator!=(const vec2&) const   -> bool;
    
    /// Length comparison
    auto operator>(const vec2&)  const   -> bool;

    auto operator>=(const vec2&) const   -> bool;

    auto operator<(const vec2&)  const   -> bool;

    auto operator<=(const vec2&) const   -> bool;

    // === Attributes ===
    double x, y;
  };
}
