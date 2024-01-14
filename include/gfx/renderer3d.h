#pragma once

#include <map>
#include <cmath>
#include <vector>
#include <algorithm>

#include "gfx/mesh.h"
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
  // ~ Set up
  auto SetPerspective(float, float, float)                      -> void;

  auto SetCameraTransform(const math::Vec3*, const math::Vec3*) -> void;

  auto SetSunDirection(const math::Vec3&)                       -> void;

  // ~ Resources
  auto LoadMesh(const char*, const char*, const char* sprite="") -> void;

  auto UnloadMesh(const char*)                                   -> void;

  // ~ Draw
  auto AddMeshToRenderHeap(const char*, const math::Vec3&,
                           const math::Vec3&, const math::Vec3&) -> void;

  auto RenderHeap() -> void;

private:
  Renderer3D();

  ~Renderer3D();

  // Structs
  struct RenderData {
    Mesh *mesh;
    math::Vec3 pos, rot, scale;
  };

  // Internal methods
  // ~ Render
  auto CalcViewMatrix()                         -> void;

  auto CalcTransformMatrix(
    const math::Vec3&,
    const math::Vec3&)                          -> math::Matrix4x4;
  
  auto FilterMeshTriangles(
    const RenderData&,
    std::vector<RawTriangle>&)                  -> void;

  auto DrawTriangles(std::vector<RawTriangle>&) -> void;

  auto DrawTriangle(RawTriangle&)               -> void;

  auto ProjectTriangle(RawTriangle&)            -> void;

  auto TriangleClipAgainstPlane(
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

  // vars
  sdl::Window *context_window_;
  struct {
    const math::Vec3* pos;
    const math::Vec3* rot;
  } camera_;

  math::Vec3 sun_direction_;

  std::map< const char*, Mesh* > meshes_;

  std::vector< RenderData > render_heap_;
  math::Matrix4x4 projmat_;
  math::Matrix4x4 viewmat_;

  static Renderer3D* kInstance;

};

}
