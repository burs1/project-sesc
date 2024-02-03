#pragma once

#include <map>
#include <vector>

#include "gfx/mesh.h"
#include "gfx/components/renderer.h"
#include "gfx/raw-triangle.h"
#include "window/drawer.h"
#include "math/vec3.h"
#include "math/matrix4x4.h"

namespace eng::app {

class App; // forward declaration to make "App"
           // a friend class of Renderer3D, so nobody else
           // couldn't instantiate it.

}

namespace eng::gfx {

class Renderer3D {
friend app::App;
public:
  // ! Not copyable
  Renderer3D(const Renderer3D&) = delete;

  Renderer3D operator=(const Renderer3D&) = delete;

  // ! Not movable
  Renderer3D(Renderer3D&&) = delete;

  Renderer3D& operator=(Renderer3D&&) = delete;

  // - Methods -
  auto LoadMesh(const char*, const char*) -> void;

  auto UnloadMesh(const char*)                                         -> void;

  auto SetPerspective(float, float, float)                      -> void;

  auto SetFOV(float)                                            -> void;

  auto SetNearPlane(float)                                      -> void;

  auto SetFarPlane(float)                                       -> void;

  auto SetPlanes(float, float)                                  -> void;

  auto SetCameraTransform(const math::Vec3*, const math::Vec3*) -> void;

  auto SetSunRotation(const math::Vec3&)                        -> void;

  auto RenderFrame() -> void;

private:
  // Constructor
  Renderer3D(window::Drawer*);

  // Destructor
  ~Renderer3D();

  // - Internal methods -
  auto CalcViewMatrix() -> void;

  auto CalcTransformMatrix(
    const math::Vec3&,
    const math::Vec3&,
    const math::Vec3&) -> math::Matrix4x4;
  
  auto ProcessTriangles(
    const Renderer*,
    std::vector<RawTriangle>&) -> void;

  auto SortTriangles(std::vector<RawTriangle>&) -> void;

  auto DrawTriangles(std::vector<RawTriangle>&) -> void;

  auto ProjectTriangle(RawTriangle&)            -> void;

  auto DrawTriangle(RawTriangle&)               -> void;

  auto ClipTriangleAgainstPlane(
    math::Vec3,
    math::Vec3,
    RawTriangle,
    RawTriangle&,
    RawTriangle&) -> int;

  auto FindPlaneIntersectionPoint(
    math::Vec3,
    math::Vec3,
    const math::Vec3&,
    const math::Vec3&,
    float&) -> math::Vec3;

  // Vars
  window::Drawer *drawer_;
  
  struct {
    const math::Vec3* pos = nullptr;
    const math::Vec3* rot = nullptr;
  } camera_;

  math::Vec3 sun_direction_;
  float fov_ = 0.0f, near_ = 0.0f, far_ = 0.0f;

  std::map<const char*, Mesh*> meshes_;

  std::vector<Renderer*> renderer_components_;
  std::vector<window::Texture*> rendered_text_;
  math::Matrix4x4 projmat_;
  math::Matrix4x4 viewmat_;

  static Renderer3D* kInstance;
};

}
