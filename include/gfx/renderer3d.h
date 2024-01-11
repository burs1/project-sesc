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
    Renderer3D(sdl::Window*);

    ~Renderer3D();

    // Methods
    // ~ Set up
    auto SetPerspective(float, float, float) -> void;

    auto SetSunDirection(const math::Vec3&)  -> void;

    // ~ Resources
    auto LoadMesh(const char*, const char*)  -> void;

    auto UnloadMesh(const char*)             -> void;

    // ~ Draw
    auto SetCameraTransform(const math::Vec3&, const math::Vec3&) -> void;

    auto AddMeshToRenderHeap(const char*, const math::Vec3&,
                             const math::Vec3&) -> void;

    auto RenderHeap() -> void;

  private:
    // Internal methods
    // ~ Render
    auto CalcViewMatrix() -> void;

    auto CalcTransformMatrix(
      const math::Vec3&,
      const math::Vec3&) -> math::Matrix4x4;
    
    auto FilterMeshTriangles(
      const Mesh*,
      const math::Vec3&,
      const math::Vec3&,
      std::vector<RawTriangle>&) -> void;

    auto DrawTriangles(std::vector<RawTriangle>&) -> void;

    auto FindPlaneIntersectionPoint(
      const math::Vec3&,
      const math::Vec3&,
      const math::Vec3&,
      const math::Vec3&) -> math::Vec3;
    
    auto FaceClipAgainstPlane(
      const math::Vec3&,
      const math::Vec3&,
      const math::Vec3[3],
      math::Vec3[3], math::Vec3[3]) -> int;

    struct RenderData {
      Mesh *mesh;
      math::Vec3 pos, rot;
    };

    // vars
    sdl::Window *context_window_;
    struct { math::Vec3 pos, rot; } cam_;

    math::Vec3 sun_direction_;

    std::map< const char*, Mesh* > meshes_;

    std::vector< RenderData > render_heap_;
    math::Matrix4x4 projmat_;
    math::Matrix4x4 viewmat_;
  };
}
