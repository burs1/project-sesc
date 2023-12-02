#include "vec2.h"

namespace engine::math {
  // === Constructors & Destructors ===
  /// Constructor
  vec2::vec2(double p_x, double p_y)
    : x(p_x), y(p_y) {}

  /// Copy Constructor
  vec2::vec2(const vec2& other)
    : x(other.x), y(other.y) {}

  // === Methods ===
  /// Vector's length
  constexpr auto vec2::magnitude() const -> double {
    return sqrt(x * x + y * y);
  }

  /// Normalize components
  auto vec2::normalize() -> void {
    double length = magnitude();
    x /= length;
    y /= length;
  }

  /// Linear interpolation
  auto vec2::lerp(const vec2& other, double value) -> void {
    x = (other.x - x) * value;
    y = (other.y - y) * value;
  }

  // ===  methods ===
  auto vec2::normalized(const vec2& v) -> vec2 {
    double length = v.magnitude();
    return vec2(v.x / length, v.y / length);
  }

  constexpr auto vec2::dot(const vec2& v1, const vec2& v2) -> double {
    return v1.x * v2.x + v1.y + v2.y;
  }

  auto vec2::lerp(const vec2& v1, const vec2& v2, double value) -> vec2 {
    return vec2((v2.x - v1.x) * value, (v2.y - v1.y) * value);  
  }


  // === Overloads ===
  /// Copy
  auto vec2::operator=(const vec2& other) -> void {
    x = other.x;
    y = other.y;
  }

  /// Summary
  auto vec2::operator+(const vec2 &other) const -> vec2 {
    return vec2(x + other.x, y + other.y);
  }

  auto vec2::operator+=(const vec2 &other) -> void {
    x += other.x;
    y += other.y;
  }

  /// Substract
  auto vec2::operator-(const vec2 &other) const -> vec2 {
    return vec2(x - other.x, y - other.y);
  }

  auto vec2::operator-=(const vec2 &other) -> void {
    x -= other.x;
    y -= other.y;
  }

  /// Scale
  auto vec2::operator*(const double &value) const -> vec2 {
    return vec2(x * value, y * value);
  }

  auto vec2::operator*=(const double &value) -> void {
    x *= value;
    y *= value;
  }

  auto vec2::operator/(const double &value) const -> vec2 {
    return vec2(x / value, y / value);
  }

  auto vec2::operator/=(const double &value) -> void {
    x /= value;
    y /= value;
  }

  /// Components comparison
  auto vec2::operator==(const vec2 &other) const -> bool {
    return x == other.x && y == other.y;
  }

  auto vec2::operator!=(const vec2 &other) const -> bool {
    return !(*this == other);
  }
 
  /// Length comparison
  auto vec2::operator>(const vec2 &other) const -> bool {
    return this->magnitude() > other.magnitude();
  }

  auto vec2::operator>=(const vec2 &other) const -> bool {
    return this->magnitude() >= other.magnitude();
  }

  auto vec2::operator<(const vec2 &other) const -> bool {
    return this->magnitude() < other.magnitude();
  }

  auto vec2::operator<=(const vec2 &other) const -> bool {
    return this->magnitude() <= other.magnitude();
  }
}
