#pragma once

#include <map>
#include <cmath>
#include <vector>
#include <stdexcept>
#include <algorithm>

#include "gfx/mesh.h"
#include "gfx/raw-triangle.h"
#include "gfx/render-data.h"
#include "gfx/mesh-render-data.h"
#include "gfx/texture-render-data.h"
#include "math/vec3.h"
#include "math/matrix4x4.h"
#include "window/window.h"

namespace eng::gfx {

class Renderer3D {
public:
  // Static methods
  static auto Init()        -> void;

  static auto GetInstance() -> Renderer3D*;

  static auto Quit()        -> void;

  // Not copyable
  Renderer3D(const Renderer3D&) = delete;

  Renderer3D operator=(const Renderer3D&) = delete;

  // Not movable
  Renderer3D(Renderer3D&&) = delete;

  Renderer3D& operator=(Renderer3D&&) = delete;

  // Methods
  // ~ Resources
  auto LoadMesh(const char*, const char*, const char* texture_name="") -> void;

  auto UnloadMesh(const char*)                                         -> void;

  // ~ Setters
  auto SetPerspective(float, float, float)                      -> void;

  auto SetFOV(float)                                            -> void;

  auto SetNearPlane(float)                                      -> void;

  auto SetFarPlane(float)                                       -> void;

  auto SetPlanes(float, float)                                  -> void;

  auto SetCameraTransform(const math::Vec3*, const math::Vec3*) -> void;

  auto SetSunRotation(const math::Vec3&)                        -> void;

  // ~ Render
  auto AddMeshToQueue(
    const math::Vec3&,
    const math::Vec3&,
    const math::Vec3&,
    const char*) -> void;

  auto AddTextureToQueue(
    const math::Vec3&,
    const math::Vec3&,
    const math::Vec3&,
    const char*) -> void;

  auto AddTextToQueue(
    const math::Vec3&,
    const math::Vec3&,
    const math::Vec3&,
    const char*,
    const char* font="") -> void;

  auto RenderQueue() -> void;

private:
  Renderer3D();

  ~Renderer3D();

  // Internal methods
  // ~ Render
  auto CalcViewMatrix() -> void;

  auto CalcTransformMatrix(
    const math::Vec3&,
    const math::Vec3&,
    const math::Vec3&) -> math::Matrix4x4;
  
  auto ProcessTriangles(
    const RenderData&,
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
  window::Window *context_window_;
  struct {
    const math::Vec3* pos;
    const math::Vec3* rot;
  } camera_;

  math::Vec3 sun_direction_;
  float fov_ = 0.0f, near_ = 0.0f, far_ = 0.0f;

  std::map<const char*, Mesh*> meshes_;
  std::map<const char*, std::pair<window::Texture*, bool>> rendered_text_;

  std::vector<RenderData> render_queue_;
  math::Matrix4x4 projmat_;
  math::Matrix4x4 viewmat_;

  static Renderer3D* kInstance;

};

}
