/*
 * Author: Ilya Buravov
 *
 * Summary: Simple 4x4 matrix implementation
 *
 */

#include "matrix4x4.h"

namespace engine::math {
  // === Constructors and destructors ===
  /// Constructor
  matrix4x4::matrix4x4() {
    set_identity();
  }

  /// Copy constructor
  matrix4x4::matrix4x4(const matrix4x4& other) {
    _copy(other);
  }

  // === Methods ===
  /// Set identity
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

  /// Set translation
  auto matrix4x4::set_translation(const vec3& v) -> void {
    m[0][0] = 1.0f;
    m[1][1] = 1.0f;
    m[2][2] = 1.0f;
    m[3][3] = 1.0f;
    m[3][0] = v.x;
    m[3][1] = v.y;
    m[3][2] = v.z;
  }

  /// Return tranlslation v
  auto matrix4x4::get_translation() -> vec3 {
    return vec3(m[3][0], m[3][1], m[3][2]);
  }

  /// Return rightward v
  auto matrix4x4::right() -> vec3 {
    return vec3(m[0][0], m[0][1], m[0][2]);
  }

  /// Return upward v
  auto matrix4x4::up() -> vec3 {
    return vec3(m[1][0], m[1][1], m[1][2]);
  }

  /// Return forward v
  auto matrix4x4::forward() -> vec3 {
    return vec3(m[2][0], m[2][1], m[2][2]);
  }

  /// Set scale
  auto matrix4x4::set_scale(const vec3& v) -> void {
    m[0][0] = v.x;
    m[1][1] = v.y;
    m[2][2] = v.z;
  }

  /// Set rotation
  auto matrix4x4::set_rot(float x, float y, float z) -> void {
    set_rotX(x);
    set_rotY(y);
    set_rotZ(z);
  }

  /// Set rotation
  auto matrix4x4::set_rot(const vec3& v) -> void {
    matrix4x4 xrot, yrot, zrot;

    xrot.set_rotX(v.x);
    yrot.set_rotY(v.y);
    zrot.set_rotZ(v.z);

    operator*=(xrot);
    operator*=(yrot);
    operator*=(zrot);
  }

  /// Set x rotation
  auto matrix4x4::set_rotX(float rot) -> void {
    m[0][0] = 1.0f;
    m[1][1] = cosf(rot);
    m[1][2] = sinf(rot);
    m[2][1] = -sinf(rot);
    m[2][2] = cosf(rot);
    m[3][3] = 1.0f;
  }
 
  /// Set y rotation
  auto matrix4x4::set_rotY(float rot) -> void {
    m[0][0] = cosf(rot);
    m[0][2] = -sinf(rot);
    m[2][0] = sinf(rot);
    m[1][1] = 1.0f;
    m[2][2] = cosf(rot);
    m[3][3] = 1.0f;
  }

  /// Set z rotation
  auto matrix4x4::set_rotZ(float rot) -> void {
    m[0][0] = cosf(rot);
    m[0][1] = sinf(rot);
    m[1][0] = -sinf(rot);
    m[1][1] = cosf(rot);
    m[2][2] = 1.0f;
    m[3][3] = 1.0f;
  }

  /// Set persprective
  auto matrix4x4::set_perspective(float fov, float aspratio, float near, float far) -> void {
    float fovrad = 1.0f / tanf(fov / 2 / 180 * 3.14159f);
		m[0][0] = aspratio * fovrad;
		m[1][1] = fovrad;
		m[2][2] = far / (far - near);
		m[2][3] = 1.0f;
		m[3][2] = (-near * far) / (far - near);
		m[3][3] = 0.0f;
	}
  
  /// Make proj matrix
  auto matrix4x4::point_at(const vec3 &from, const vec3 &to, const vec3 &up) -> void {
    vec3 newforward = vec3::normalized(to - from);
    vec3 newup = vec3::normalized(up - newforward * vec3::dot(up, newforward));
    vec3 newright = vec3::cross(newup, newforward);

    m[0][0] = newright.x; m[0][1] = newright.y; m[0][2] = newright.z;
    m[1][0] = newup.x; m[1][1] = newup.y; m[1][2] = newup.z;
    m[2][0] = newforward.x; m[2][1] = newforward.y; m[2][2] = newforward.z;
    m[3][0] = from.x; m[3][1] = from.y; m[3][2] = from.z;
  }

  /// Make look at matrix
  auto matrix4x4::look_at(const vec3 &from, const vec3 &to, const vec3 &up) -> void {
    point_at(from, to, up);
    inverse();
  }

  /// Inverse
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

  auto matrix4x4::_copy(const matrix4x4& other) -> void {
	  for (int i = 0; i < 4; ++i) {
	    for (int j = 0; j < 4; ++j)
	      m[i][j] = other.m[i][j];
	  }
	}

	// === Overloads ===
	/// Mult
	auto matrix4x4::operator*=(const matrix4x4& other) -> void {
	  _copy( operator*(other) );
	}
  
  auto matrix4x4::operator*(const matrix4x4& other) -> matrix4x4 {
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

	/// Copy
	auto matrix4x4::operator=(const matrix4x4& other) -> void {
	  _copy(other);
	}
}
