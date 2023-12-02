/*
 * Author: Ilya Buravov
 * 
 * Summary: Simple 4x4 matrix implementation
 *
 */

#pragma once

#include "vec3.h"

namespace engine::math {
  class vec3;
  class matrix4x4 {
  public:
    // === Constructors and destructors ===
    /// Constructor
    matrix4x4();

    /// Copy constructor
    matrix4x4(const matrix4x4&);

    // === Methods ===
    /// Set "zero rotation" matrix
    auto set_identity()                  -> void;

    /// Set translation by vector
    auto set_translation(const vec3&) -> void;

    /// Return translation vector
    auto get_translation()               -> vec3;

    /// Return rightward vector
    auto right()                        -> vec3;

    /// Return upward vector
    auto up()                           -> vec3;

    /// Return forward vector
    auto forward()                      -> vec3;

    /// Set scale by vector
    auto set_scale(const vec3&)          -> void;

    /// Set rotation
    auto set_rot(float, float, float) -> void;

    /// Set rotation
    auto set_rot(const vec3&)            -> void;

    /// Set x rotation
    auto set_rotX(float)                -> void;
    
    /// Set y rotation
    auto set_rotY(float)                -> void;

    /// Set x rotation
    auto set_rotZ(float)                -> void;

    /// Set perspective 
    auto set_perspective(float, float, float, float) -> void;

    // === Overloads ===
    /// Copy
    auto operator=(const matrix4x4&)    -> void;

    float m[4][4];

  private:
    /// Copy m from other matrix
    auto __copy__(const matrix4x4&)     -> void;
  };
}
