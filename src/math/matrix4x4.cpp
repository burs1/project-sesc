/*
 * Author: Ilya Buravov
 *
 * Summary: Simple 4x4 matrix implementation
 *
 */

#include "matrix4x4.h"

namespace engine::math {
  matrix4x4::matrix4x4() { set_identity(); }

  matrix4x4::matrix4x4(const matrix4x4& other) { _copy(other); }

  // methods
  auto matrix4x4::set_identity() -> void {
    // fill matrix 
    for (int i = 0; i < 4; ++i) {
      for (int j = 0; j < 4; ++j)
        m[i][j] = i == j;
    }

    // final matrix:
    // [1, 0, 0, 0]
    // [0, 1, 0, 0]
    // [0, 0, 1, 0]
    // [0, 0, 0, 1]
  }

  auto matrix4x4::set_translation(const vec3& v) -> void {
    m[0][0] = 1.0f;
    m[1][1] = 1.0f;
    m[2][2] = 1.0f;
    m[3][3] = 1.0f;
    m[3][0] = v.x;
    m[3][1] = v.y;
    m[3][2] = v.z;
  }

  auto matrix4x4::get_translation() const -> vec3 {
    return vec3(m[3][0], m[3][1], m[3][2]);
  }

  auto matrix4x4::right() const -> vec3 {
    return vec3(m[0][0], m[0][1], m[0][2]);
  }

  auto matrix4x4::up() const -> vec3 {
    return vec3(m[1][0], m[1][1], m[1][2]);
  }

  auto matrix4x4::forward() const -> vec3 {
    return vec3(m[2][0], m[2][1], m[2][2]);
  }

  auto matrix4x4::set_scale(const vec3& v) -> void {
    m[0][0] = v.x;
    m[1][1] = v.y;
    m[2][2] = v.z;
  }

  auto matrix4x4::set_rot(const vec3& v) -> void {
    matrix4x4 xrot, yrot, zrot;

    xrot.set_rotX(v.x);
    yrot.set_rotY(v.y);
    zrot.set_rotZ(v.z);

    operator*=(xrot);
    operator*=(yrot);
    operator*=(zrot);
  }

  auto matrix4x4::set_rotX(float rot) -> void {
    m[0][0] = 1.0f;
    m[1][1] = cosf(rot);
    m[1][2] = sinf(rot);
    m[2][1] = -sinf(rot);
    m[2][2] = cosf(rot);
    m[3][3] = 1.0f;
  }
 
  auto matrix4x4::set_rotY(float rot) -> void {
    m[0][0] = cosf(rot);
    m[0][2] = -sinf(rot);
    m[2][0] = sinf(rot);
    m[1][1] = 1.0f;
    m[2][2] = cosf(rot);
    m[3][3] = 1.0f;
  }

  auto matrix4x4::set_rotZ(float rot) -> void {
    m[0][0] = cosf(rot);
    m[0][1] = sinf(rot);
    m[1][0] = -sinf(rot);
    m[1][1] = cosf(rot);
    m[2][2] = 1.0f;
    m[3][3] = 1.0f;
  }

  auto matrix4x4::set_perspective(float fov, float aspratio, float near, float far) -> void {
    float fovrad = 1.0f / tanf( (fov / 2) * (3.14159f / 180) );
		m[0][0] = aspratio * fovrad;
		m[1][1] = fovrad;
		m[2][2] = far / (far - near);
		m[2][3] = 1.0f;
		m[3][2] = -near * m[2][2];
		m[3][3] = 0.0f;
	}
  
  auto matrix4x4::point_at(const vec3 &from, const vec3 &to, const vec3 &up) -> void {
    vec3 newforward = to - from;
    newforward.normalize();

    vec3 newup = up - newforward * vec3::dot(up, newforward);
    newup.normalize();

    vec3 newright = vec3::cross(newup, newforward);

    m[0][0] = newright.x;   m[0][1] = newright.y;   m[0][2] = newright.z;   m[0][3] = 0.0f;
    m[1][0] = newup.x;      m[1][1] = newup.y;      m[1][2] = newup.z;      m[1][3] = 0.0f;
    m[2][0] = newforward.x; m[2][1] = newforward.y; m[2][2] = newforward.z; m[2][3] = 0.0f;
    m[3][0] = from.x;       m[3][1] = from.y;       m[3][2] = from.z;       m[3][3] = 1.0f;
  }

  auto matrix4x4::look_at(const vec3 &from, const vec3 &to, const vec3 &up) -> void {
    point_at(from, to, up);
    inverse();
  }

  auto matrix4x4::inverse() -> void {
    matrix4x4 res;
		res.m[0][0] = m[0][0]; res.m[0][1] = m[1][0]; res.m[0][2] = m[2][0]; res.m[0][3] = 0.0f;
		res.m[1][0] = m[0][1]; res.m[1][1] = m[1][1]; res.m[1][2] = m[2][1]; res.m[1][3] = 0.0f;
		res.m[2][0] = m[0][2]; res.m[2][1] = m[1][2]; res.m[2][2] = m[2][2]; res.m[2][3] = 0.0f;

    res.m[3][0] = -(m[3][0] * res.m[0][0] + m[3][1] * res.m[1][0] + m[3][2] * res.m[2][0]);
		res.m[3][1] = -(m[3][0] * res.m[0][1] + m[3][1] * res.m[1][1] + m[3][2] * res.m[2][1]);
		res.m[3][2] = -(m[3][0] * res.m[0][2] + m[3][1] * res.m[1][2] + m[3][2] * res.m[2][2]);

    res.m[3][3] = 1.0f;

		_copy(res);
  }


  // operators
	auto matrix4x4::operator*=(const matrix4x4& other) -> void {
	  _copy( operator*(other) );
	}
  
  auto matrix4x4::operator*(const matrix4x4& other) const -> matrix4x4 {
    matrix4x4 res;

    for (int c = 0; c < 4; c++) {
			for (int r = 0; r < 4; r++) {
			  res.m[r][c] = m[r][0] * other.m[0][c] +
			                m[r][1] * other.m[1][c] +
			                m[r][2] * other.m[2][c] + 
			                m[r][3] * other.m[3][c];
			}
		}

		return res;
  }

	auto matrix4x4::operator=(const matrix4x4& other) -> void {
	  _copy(other);
	}

  // internal methods
  auto matrix4x4::_copy(const matrix4x4& other) -> void {
	  for (int i = 0; i < 4; ++i) {
	    for (int j = 0; j < 4; ++j) { m[i][j] = other.m[i][j]; }
	  }
	}

}
