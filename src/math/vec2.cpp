#include "math/vec2.h"

namespace eng::math {
  Vec2::Vec2()
    : x(0.0f), y(0.0f) {}

  Vec2::Vec2(float x, float y)
    : x(x), y(y) {}

  Vec2::Vec2(const Vec2& other)
    : x(other.x), y(other.y) {}

  auto Vec2::Magnitude() const -> float {
    return sqrtf(x * x + y * y);
  }

  auto Vec2::Normalize() -> void {
    float length = Magnitude();
    x /= length;
    y /= length;
  }

  auto Vec2::Lerp(const Vec2& other, float value) -> void {
    x = x + (other.x - x) * value;
    y = y + (other.y - y) * value;
 }

  auto Vec2::Normalized(const Vec2& v) -> Vec2 {
    float length = v.Magnitude();
    return Vec2(
      v.x / length,
      v.y / length
    );
  } 

  auto Vec2::Dot(const Vec2& v1, const Vec2& v2) -> float {
    return v1.x * v2.x + v1.y * v2.y;
  }

  auto Vec2::Lerp(const Vec2& v1, const Vec2& v2, float value)  -> Vec2 {
    return Vec2(
      v1.x + (v2.x - v1.x) * value,
      v1.y + (v2.y - v1.y) * value
    );
  }

  auto Vec2::operator=(const Vec2& other) -> void {
    x = other.x;
    y = other.y;
  }

  auto Vec2::operator+(const Vec2 &other) const -> Vec2 {
    return Vec2(x + other.x, y + other.y);
  }

  auto Vec2::operator+=(const Vec2 &other) -> void {
    x += other.x;
    y += other.y;
  }

  auto Vec2::operator-(const Vec2 &other) const -> Vec2 {
    return Vec2(x - other.x, y - other.y);
  }

  auto Vec2::operator-=(const Vec2 &other) -> void {
    x -= other.x;
    y -= other.y;
  }

  auto Vec2::operator*(const float &value) const -> Vec2 {
    return Vec2(x * value, y * value);
  }

  auto Vec2::operator*=(const float &value) -> void {
    x *= value;
    y *= value;
  }

  auto Vec2::operator*=(const Vec2& other) -> void {
    x *= other.x;
    y *= other.y;
  }

  auto Vec2::operator*(const Vec2& other) const -> Vec2 {
    return Vec2(x * other.x, y * other.y);
  }


  auto Vec2::operator/(const float &value) const -> Vec2 {
    return Vec2(x / value, y / value);
  }

  auto Vec2::operator/=(const float &value) -> void {
    x /= value;
    y /= value;
  }

  auto Vec2::operator==(const Vec2 &other) const -> bool {
    return x == other.x && y == other.y;
  }

  auto Vec2::operator!=(const Vec2 &other) const -> bool {
    return !(*this == other);
  }

  auto Vec2::operator>(const Vec2 &other) const -> bool {
    return this->Magnitude() > other.Magnitude();
  }

  auto Vec2::operator>=(const Vec2 &other) const -> bool {
    return this->Magnitude() >= other.Magnitude();
  }

  auto Vec2::operator<(const Vec2 &other) const -> bool {
    return this->Magnitude() < other.Magnitude();
  }

  auto Vec2::operator<=(const Vec2 &other) const -> bool {
    return this->Magnitude() <= other.Magnitude();
  }
}
