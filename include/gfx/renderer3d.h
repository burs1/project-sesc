#pragma once

#include <map>
#include <vector>
#include <string>

#include "gfx/mesh.h"
#include "gfx/components/renderer.h"
#include "gfx/raw-triangle.h"
#include "window/drawer.h"
#include "math/vec3.h"
#include "math/matrix4x4.h"
#include "gmpl/scene.h"

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

  // Loads a file into a Mesh object and inserts it in map.
  // with given name.
  auto LoadMesh(std::string, std::string)                       -> void;

  // Unloads mesh object by it's name.
  auto UnloadMesh(std::string)                                  -> void;

  // Returns mesh by it's name.
  auto GetMesh(std::string)                                     -> Mesh*;

  // Calcs new projection matrix based on passed fov,
  // near and far plane distance.
  auto SetPerspective(float, float, float)                      -> void;

  // Calcs new projection matrix based on passed fov and
  // current near and far plane distance values.
  auto SetFOV(float)                                            -> void;

  // Calcs new projection matrix based on passed near plane distance
  // and current fov and far plane distance.
  auto SetNearPlane(float)                                      -> void;

  // Calcs new projection matrix based on passed far plane distance
  // and current fov and near plane distance.
  auto SetFarPlane(float)                                       -> void;

  // Calcs new projection matrix based on passed far and near
  // plane distances and current fov.
  auto SetPlanes(float, float)                                  -> void;

  // Sets pointers to two vectors that represent position
  // and rotation of the camera.
  auto SetCameraTransform(const math::Vec3*, const math::Vec3*) -> void;

  // Sets sun rotation.
  auto SetSunRotation(const math::Vec3&)                        -> void;

  // Sets sun color.
  auto SetSunColor(Uint8, Uint8, Uint8)                         -> void;
  
  // Renders frame.
  auto RenderFrame() -> void;

 private:
  Renderer3D(window::Drawer*);
  ~Renderer3D();

  // Sets pointer to a map that contains pointers to renderer components.
  auto SetRendererComponentsMap(std::map<int, Renderer*>*) -> void;

  // Calcs view matrix based on user passed pointers to pos and rot
  // of the camera. If pointers weren't set default values will be used.
  auto CalcViewMatrix() -> void;

  //
  auto ProcessTriangles(
    const Renderer*,
    std::vector<RawTriangle>&) -> void;

  // Sorts triangles by distance to the camera.
  auto SortTriangles(std::vector<RawTriangle>&) -> void;

  // Clips, projects and draws triangles.
  auto DrawTriangles(std::vector<RawTriangle>&) -> void;

  // Projects triangle verts.
  auto ProjectTriangle(RawTriangle&)            -> void;

  // Draws projected triangle.
  auto DrawTriangle(RawTriangle&)               -> void;

  // Clips triangle against view frustum.
  auto ClipTriangleAgainstPlane(
    math::Vec3,
    math::Vec3,
    RawTriangle,
    RawTriangle&,
    RawTriangle&) -> int;

  // Finds interection point of view frustum and triangle.
  auto FindPlaneIntersectionPoint(
    math::Vec3,
    math::Vec3,
    const math::Vec3&,
    const math::Vec3&,
    float&) -> math::Vec3;

  gmpl::Scene    *scene_;
  window::Drawer *drawer_;
  
  struct {
    float fov = 0.0f, near = 0.0f, far = 0.0f;

    const math::Vec3* pos = nullptr;
    const math::Vec3* rot = nullptr;
  } camera_;
  math::Matrix4x4 projmat_;
  math::Matrix4x4 viewmat_;
  math::Matrix4x4 view_rotmat_;

  math::Vec3 sun_direction_;
  Uint8 sun_color_[3] = {255, 255, 255};

  std::map<std::string, Mesh*> meshes_;
  std::map<int, Renderer*> *renderer_components_;
};

}

