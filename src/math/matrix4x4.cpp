#include <cmath>
#include <numbers>
#include "math/matrix4x4.h"

namespace eng::math {

Matrix4x4::Matrix4x4() { SetIdentity(); }


Matrix4x4::Matrix4x4(const Matrix4x4& other) { Copy(other); }


auto Matrix4x4::SetIdentity() -> void
{
  // Fill matrix 
  for (int i = 0; i < 4; ++i)
    for (int j = 0; j < 4; ++j) { m[i][j] = i == j; }

  // Final matrix:
  // [1, 0, 0, 0]
  // [0, 1, 0, 0]
  // [0, 0, 1, 0]
  // [0, 0, 0, 1]
}


auto Matrix4x4::SetTranslation(const Vec3& v) -> void
{
  SetIdentity();
  m[0][0] = 1.0f;
  m[1][1] = 1.0f;
  m[2][2] = 1.0f;
  m[3][3] = 1.0f;
  m[3][0] = v.x;
  m[3][1] = v.y;
  m[3][2] = v.z;
}


auto Matrix4x4::SetRotation(const Vec3& v) -> void
{
  Matrix4x4 mat;

  SetRotationX(v.x);
  
  mat.SetRotationY(v.y);
  *this *= mat;

  mat.SetRotationZ(v.z);
  *this *= mat;
}


auto Matrix4x4::SetRotationX(float rot) -> void
{
  rot *= std::numbers::pi * 2;
  SetIdentity();
  m[0][0] = 1.0f;
  m[1][1] = cosf(rot);
  m[1][2] = sinf(rot);
  m[2][1] = -sinf(rot);
  m[2][2] = cosf(rot);
  m[3][3] = 1.0f;
}


auto Matrix4x4::SetRotationY(float rot) -> void
{
  rot *= std::numbers::pi * 2;
  SetIdentity();
  m[0][0] = cosf(rot);
  m[0][2] = -sinf(rot);
  m[2][0] = sinf(rot);
  m[1][1] = 1.0f;
  m[2][2] = cosf(rot);
  m[3][3] = 1.0f;
}


auto Matrix4x4::SetRotationZ(float rot) -> void
{
  rot *= std::numbers::pi * 2;
  SetIdentity();
  m[0][0] = cosf(rot);
  m[0][1] = sinf(rot);
  m[1][0] = -sinf(rot);
  m[1][1] = cosf(rot);
  m[2][2] = 1.0f;
  m[3][3] = 1.0f;
}


auto Matrix4x4::SetScale(const Vec3& v) -> void
{
  SetIdentity();
  m[0][0] = v.x;
  m[1][1] = v.y;
  m[2][2] = v.z;
}


auto Matrix4x4::SetTransform(
    const Vec3& pos,
    const Vec3& rot,
    const Vec3& scale) -> void
{
  Matrix4x4 mat;
  SetScale(scale);

  mat.SetRotation(rot);
  *this *= mat;

  mat.SetTranslation(pos);
  *this *= mat;
}


auto Matrix4x4::Right() const -> Vec3 {
  return Vec3(m[0][0], m[0][1], m[0][2]);
}


auto Matrix4x4::Up() const -> Vec3 {
  return Vec3(m[1][0], m[1][1], m[1][2]);
}


auto Matrix4x4::Forward() const -> Vec3 {
  return Vec3(m[2][0], m[2][1], m[2][2]);
}


auto Matrix4x4::SetPerspective(float fov, float aspratio,
                               float near, float far) -> void
{
  float fovrad = 1 / tanf( (fov / 2) * (3.14159f / 180) );
	m[0][0] = fovrad / aspratio;
	m[1][1] = fovrad;
	m[2][2] = far / (far - near);
	m[2][3] = 1.0f;
	m[3][2] = -near * m[2][2];
	m[3][3] = 0.0f;
}


auto Matrix4x4::PointAt(const Vec3 &from, const Vec3 &to,
                        const Vec3 &up) -> void
{
  Vec3 newforward = to - from;
  newforward.Normalize();

  Vec3 newup = up - newforward * Vec3::Dot(up, newforward);
  newup.Normalize();

  Vec3 newright = Vec3::Cross(newup, newforward);

  m[0][0] = newright.x;   m[0][1] = newright.y;   m[0][2] = newright.z;   m[0][3] = 0.0f;
  m[1][0] = newup.x;      m[1][1] = newup.y;      m[1][2] = newup.z;      m[1][3] = 0.0f;
  m[2][0] = newforward.x; m[2][1] = newforward.y; m[2][2] = newforward.z; m[2][3] = 0.0f;
  m[3][0] = from.x;       m[3][1] = from.y;       m[3][2] = from.z;       m[3][3] = 1.0f;
}


auto Matrix4x4::LookAt(const Vec3 &from, const Vec3 &to,
                       const Vec3 &up) -> void {
  PointAt(from, to, up);
  Inverse();
}


auto Matrix4x4::Inverse() -> void
{
  Matrix4x4 res;
	res.m[0][0] = m[0][0]; res.m[0][1] = m[1][0]; res.m[0][2] = m[2][0]; res.m[0][3] = 0.0f;
	res.m[1][0] = m[0][1]; res.m[1][1] = m[1][1]; res.m[1][2] = m[2][1]; res.m[1][3] = 0.0f;
	res.m[2][0] = m[0][2]; res.m[2][1] = m[1][2]; res.m[2][2] = m[2][2]; res.m[2][3] = 0.0f;

  res.m[3][0] = -(m[3][0] * res.m[0][0] + m[3][1] * res.m[1][0] + m[3][2] * res.m[2][0]);
	res.m[3][1] = -(m[3][0] * res.m[0][1] + m[3][1] * res.m[1][1] + m[3][2] * res.m[2][1]);
	res.m[3][2] = -(m[3][0] * res.m[0][2] + m[3][1] * res.m[1][2] + m[3][2] * res.m[2][2]);

  res.m[3][3] = 1.0f;

	Copy(res);
}


auto Matrix4x4::operator*=(const Matrix4x4& other) -> void {
	Copy( operator*(other) );
}


auto Matrix4x4::operator*(const Matrix4x4& other) const -> Matrix4x4
{
  Matrix4x4 res;
  for (int c = 0; c < 4; c++)
		for (int r = 0; r < 4; r++)
			res.m[r][c] = m[r][0] * other.m[0][c] +
			              m[r][1] * other.m[1][c] +
			              m[r][2] * other.m[2][c] + 
			              m[r][3] * other.m[3][c];
	return res;
}


auto Matrix4x4::operator=(const Matrix4x4& other) -> void {
	Copy(other);
}


auto Matrix4x4::Copy(const Matrix4x4& other) -> void {
	for (int i = 0; i < 4; ++i)
	  for (int j = 0; j < 4; ++j) { m[i][j] = other.m[i][j]; }
}

}

