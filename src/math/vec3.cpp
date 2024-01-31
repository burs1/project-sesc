#include <cmath>

#include "math/vec3.h"

namespace eng::math {

Vec3::Vec3()
  : x(0.0f), y(0.0f), z(0.0f) {}

Vec3::Vec3(float x, float y, float z)
  : x(x), y(y), z(z) {}

Vec3::Vec3(const Vec3& other)
  : x(other.x), y(other.y), z(other.z) {}

auto Vec3::Magnitude() const -> float {
  return sqrtf(x * x + y * y + z * z);
}

auto Vec3::Normalize() -> void {
  float length = Magnitude();
  x /= length;
  y /= length;
  z /= length;
}

auto Vec3::Lerp(const Vec3& other, float value) -> void {
  x = x + (other.x - x) * value;
  y = y + (other.y - y) * value;
  z = z + (other.z - z) * value;
}

auto Vec3::Normalized(const Vec3& v) -> Vec3 {
  float length = v.Magnitude();
  return Vec3(
    v.x / length,
    v.y / length,
    v.z / length
  );
} 

auto Vec3::Dot(const Vec3& v1, const Vec3& v2) -> float {
  return v1.x * v2.x + v1.y * v2.y + v1.z * v2.z;
}

auto Vec3::Cross(const Vec3& v1, const Vec3& v2) -> Vec3 {
  return Vec3(
    v1.y * v2.z - v1.z * v2.y,
    v1.z * v2.x - v1.x * v2.z,
    v1.x * v2.y - v1.y * v2.x
  );
}

auto Vec3::Lerp(const Vec3& v1, const Vec3& v2, float value)  -> Vec3 {
  return Vec3(
    v1.x + (v2.x - v1.x) * value,
    v1.y + (v2.y - v1.y) * value,
    v1.z + (v2.z - v1.z) * value
  );
}

auto Vec3::operator=(const Vec3& other) -> void {
  x = other.x;
  y = other.y;
  z = other.z;
}

auto Vec3::operator+(const Vec3 &other) const -> Vec3 {
  return Vec3(x + other.x, y + other.y, z + other.z);
}

auto Vec3::operator+=(const Vec3 &other) -> void {
  x += other.x;
  y += other.y;
  z += other.z;
}

auto Vec3::operator-(const Vec3 &other) const -> Vec3 {
  return Vec3(x - other.x, y - other.y, z - other.z);
}

auto Vec3::operator-=(const Vec3 &other) -> void {
  x -= other.x;
  y -= other.y;
  z -= other.z;
}

auto Vec3::operator*(const float &value) const -> Vec3 {
  return Vec3(x * value, y * value, z * value);
}

auto Vec3::operator*=(const float &value) -> void {
  x *= value;
  y *= value;
  z *= value;
}

auto Vec3::operator*=(const Vec3& other) -> void {
  x *= other.x;
  y *= other.y;
  z *= other.z;
}

auto Vec3::operator*(const Vec3& other) const -> Vec3 {
  return Vec3(x * other.x, y * other.y, z * other.z);
}


auto Vec3::operator/(const float &value) const -> Vec3 {
  return Vec3(x / value, y / value, z / value);
}

auto Vec3::operator/=(const float &value) -> void {
  x /= value;
  y /= value;
  z /= value;
}

auto Vec3::operator*=(const Matrix4x4 &mtrx)       -> void {
  Vec3 res = operator*(mtrx);
  x = res.x;
  y = res.y;
  z = res.z;
}

auto Vec3::operator*(const Matrix4x4 &mtrx) -> Vec3 {
  Vec3 res;
  res.x = x * mtrx.m[0][0] + y * mtrx.m[1][0] + z * mtrx.m[2][0] + mtrx.m[3][0];
  res.y = x * mtrx.m[0][1] + y * mtrx.m[1][1] + z * mtrx.m[2][1] + mtrx.m[3][1];
  res.z = x * mtrx.m[0][2] + y * mtrx.m[1][2] + z * mtrx.m[2][2] + mtrx.m[3][2];
  float w = x * mtrx.m[0][3] + y * mtrx.m[1][3] + z * mtrx.m[2][3] + mtrx.m[3][3];

  if (w != 0.0f) {
    res.x /= w;
    res.y /= w;
    res.z /= w;
  }

  return res;
}

auto Vec3::operator==(const Vec3 &other) const -> bool {
  return x == other.x && y == other.y && z == other.z;
}

auto Vec3::operator!=(const Vec3 &other) const -> bool {
  return !(*this == other);
}

auto Vec3::operator>(const Vec3 &other) const -> bool {
  return this->Magnitude() > other.Magnitude();
}

auto Vec3::operator>=(const Vec3 &other) const -> bool {
  return this->Magnitude() >= other.Magnitude();
}

auto Vec3::operator<(const Vec3 &other) const -> bool {
  return this->Magnitude() < other.Magnitude();
}

auto Vec3::operator<=(const Vec3 &other) const -> bool {
  return this->Magnitude() <= other.Magnitude();
}

}

namespace std {

string to_string(const eng::math::Vec3& v) {
  return '(' + to_string(v.x) + "; " + to_string(v.y) + "; " + to_string(v.z) + ')';
}

}

