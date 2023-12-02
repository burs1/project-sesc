/*
 * Author: Ilya Buravov
 *
 * Summary: Simple 4x4 matrix implementation
 *
 */

#include "matrix4x4.h"

#include <cmath>

namespace engine::math {
  // === Constructors and destructors ===
  /// Constructor
  matrix4x4::matrix4x4() {
    set_identity();
  }

  /// Copy constructor
  matrix4x4::matrix4x4(const matrix4x4& other) {
    __copy__(other);
  }

  // === Methods ===
  /// Set identity
  auto matrix4x4::set_identity() -> void {
    // fill matrix 
    for (int i = 0; i < 4; ++i) {
      for (int j = 0; j < 4; ++j)
        m[i][j] = i == j ? 1 : 0;
    }

    // final matrix:
    // [1, 0, 0, 0]
    // [0, 1, 0, 0]
    // [0, 0, 1, 0]
    // [0, 0, 0, 1]
  }

  /// Set translation
  auto matrix4x4::set_translation(const vec3& v) -> void {
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
    set_rotX(v.x);
    set_rotY(v.y);
    set_rotZ(v.z);
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
	auto matrix4x4::__copy__(const matrix4x4& other) -> void {
	  for (int i = 0; i < 4; ++i) {
	    for (int j = 0; j < 4; ++j)
	      m[i][j] = other.m[i][j];
	  }
	}

	// === Overloads ===
	/// Copy
	auto matrix4x4::operator=(const matrix4x4& other) -> void {
	  __copy__(other);
	}
}
