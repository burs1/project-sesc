#include <vector>
#include <fstream>
#include <algorithm>
#include <strstream>
#include <stdexcept>

#include "math/vec3.h"
#include "math/vec2.h"
#include "gfx/mesh.h"
#include "gfx/triangle.h"

namespace eng::gfx {

// Constructor
Mesh::Mesh(std::string file) {
  LoadFromOBJ(file);
}


// Destructor
Mesh::~Mesh() {
  delete[] verts_;
  delete[] triangles_;
  delete[] uv_coords_;
}


// Methods
auto Mesh::GetVerts(int *verts_count) const -> math::Vec3*
{
  *verts_count = verts_count_;
  math::Vec3* arr = new math::Vec3[verts_count_];
  std::copy(verts_, verts_ + verts_count_, arr);
  return arr;
}


auto Mesh::GetUVCoords(int *uv_coords_count) const -> const math::Vec2* {
  *uv_coords_count = uv_coords_count_;
  return uv_coords_;
}


auto Mesh::GetTriangles(int *triangles_count) const -> const Triangle* {
  *triangles_count = triangles_count_;
  return triangles_;
}


auto Mesh::GetNormals(int *normals_count) const -> math::Vec3* {
  *normals_count = normals_count_;
  math::Vec3* arr = new math::Vec3[normals_count_];
  std::copy(normals_, normals_ + normals_count_, arr);
  return arr;
}


// Internal methods
// ~ OBJ
auto Mesh::LoadFromOBJ(std::string file) -> void {
  // Read file content
  std::vector<math::Vec3> new_verts;
  std::vector<math::Vec2> new_uv_coords;
  std::vector<math::Vec3> new_normals;
  std::vector<Triangle> new_triangles;

  ReadOBJ(file, new_verts, new_uv_coords, new_normals,  new_triangles);
  CopyDataFromVectors(new_verts, new_uv_coords, new_normals, new_triangles);
 
  // Vectors free
}


auto Mesh::ReadOBJ(
    std::string file,
    std::vector<math::Vec3>& new_verts,
    std::vector<math::Vec2>& new_uv_coords,
    std::vector<math::Vec3>& new_normals,
    std::vector<Triangle>& new_triangles) -> void
{
  std::ifstream in(file);

  // Check if file was successfully loaded
  if (!in.is_open())
    throw std::runtime_error("File \"" + std::string(file) + "\"not found");

  char line[128];
  char junk;

  while (!in.eof()) {
    // Read line
    in.getline(line, 128); 
    std::strstream strstream;
    strstream << line;

    // Parse data
    // ~ Vertecies
    if (line[0] == 'v') {
      switch (line[1]) {
        case 't': { // UV coords
          math::Vec2 v;
          strstream >> junk >> junk >> v.x >> v.y;
          new_uv_coords.push_back(v);
          break;
        }
        case 'n': { // Normal
          math::Vec3 v;
          strstream >> junk >> junk >> v.x >> v.y >> v.z;
          new_normals.push_back(v);
          break;
        }
        default: { // Verticies
          math::Vec3 v;
          strstream >> junk >> v.x >> v.y >> v.z;
          new_verts.push_back(v);
        }
      }
    }
    
    // ~ Triangles
    if (line[0] == 'f') {
      strstream >> junk;
      std::string tokens[9];
      int tokens_count = -1;

      // Tokens are formated like that:
      // <vert>/<uv coord>/<normal> 

      while (not strstream.eof()) {
        char c = strstream.get();
        if (c == ' ' or c == '/')
          tokens_count++;
        else
          tokens[tokens_count].append(1, c);
      }

      tokens[tokens_count].pop_back();
      new_triangles.push_back(
        Triangle{
          {
            std::stoi(tokens[0]) - 1,
            std::stoi(tokens[3]) - 1,
            std::stoi(tokens[6]) - 1
          },
          {
            std::stoi(tokens[1]) - 1,
            std::stoi(tokens[4]) - 1,
            std::stoi(tokens[7]) - 1
          },
          std::stoi(tokens[2]) - 1,
          { 255, 255, 255 }
        }
      );
    }
  }
  in.close();
}


// ~ Copy
auto Mesh::CopyDataFromVectors(
    std::vector<math::Vec3>& new_verts,
    std::vector<math::Vec2>& new_uv_coords,
    std::vector<math::Vec3>& new_normals,
    std::vector<Triangle>& new_triangles) -> void {
  // Copy verts data
  verts_count_ = new_verts.size();
  verts_ = new math::Vec3[verts_count_];
  std::copy(new_verts.begin(), new_verts.end(), verts_);

  // Copy triangles data
  triangles_count_ = new_triangles.size();
  triangles_ = new Triangle[triangles_count_];
  std::copy(new_triangles.begin(), new_triangles.end(), triangles_);

  // Copy normals data
  normals_count_ = new_normals.size();
  normals_ = new math::Vec3[normals_count_];
  std::copy(new_normals.begin(), new_normals.end(), normals_);

  // Copy uv coords data
  uv_coords_count_ = new_uv_coords.size();
  uv_coords_ = new math::Vec2[uv_coords_count_];
  std::copy(new_uv_coords.begin(), new_uv_coords.end(), uv_coords_);
}

}
