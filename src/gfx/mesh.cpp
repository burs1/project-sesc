#include "gfx/mesh.h"

namespace eng::gfx {
  Mesh::Mesh(const char* file, const char* texture_name)
    : texture_name_(texture_name) {
    LoadFromOBJ(file);
  }


  Mesh::~Mesh() {
    delete[] verts_;
    delete[] triangles_;
    delete[] uv_coords_;
  }


  // Methods
  // ~ Getters
  auto Mesh::GetVerts(int* verts_count) const -> math::Vec3* {
    math::Vec3* arr = new math::Vec3[verts_count_];

    *verts_count = verts_count_;
    std::copy(verts_, verts_ + verts_count_, arr);

    return arr;
  }

  auto Mesh::GetUVCoords(int* uv_coords_count) const -> const math::Vec2* {
    *uv_coords_count = uv_coords_count_;
    return uv_coords_;
  }

  auto Mesh::GetTriangles(int* triangles_count) const -> const Triangle* {
    *triangles_count = triangles_count_;
    return triangles_;
  }

  auto Mesh::GetTextureName() const -> const char* {
    return texture_name_;
  }


  // Internal methods
  // ~ OBJ
  auto Mesh::LoadFromOBJ(const char *file) -> void {
    std::ifstream in(file);

    // Check if file was successfully loaded
    if (!in.is_open()) {
      throw std::runtime_error("File \"" + std::string(file) + "\"not found");
    }

    // Read file content
    std::vector<math::Vec3> new_verts;
    std::vector<math::Vec2> new_uv_coords;
    std::vector<Triangle> new_triangles;

    ReadOBJ(in, new_verts, new_uv_coords, new_triangles);
    in.close();

    CopyDataFromVectors(new_verts, new_uv_coords, new_triangles);
   
    // Vectors free
  }


  auto Mesh::ReadOBJ(
      std::ifstream& in,
      std::vector<math::Vec3>& new_verts,
      std::vector<math::Vec2>& new_uv_coords,
      std::vector<Triangle>& new_triangles) -> void {
    char line[128];
    char junk;

    while (!in.eof()) {
      // Read line
      in.getline(line, 128); 
      std::strstream sstream;
      sstream << line;

      // Parse data
      // ~ Vertecies
      if (line[0] == 'v') {
        if (line[1] == 't') {
          math::Vec2 v;
          sstream >> junk >> junk >> v.x >> v.y;

          new_uv_coords.push_back(v);
          continue;
        }
        
        math::Vec3 v;
        sstream >> junk >> v.x >> v.y >> v.z;

        new_verts.push_back(v);
        continue;
      }
      
      // ~ Triangles
      if (line[0] == 'f') {
        sstream >> junk;
        
        if (new_uv_coords.size() == 0) {
          int vertsIds[3];
          sstream >> vertsIds[0] >> vertsIds[1] >> vertsIds[2];
          
          new_triangles.push_back(
            Triangle{
              {
                vertsIds[0] - 1,
                vertsIds[1] - 1,
                vertsIds[2] - 1
              },
              { 0, 0, 0 },
              { 255, 255, 255 }
            }
          );
          continue;
        }


        std::string tokens[6];
        int tokens_count = -1;

        while (not sstream.eof()) {
          char c = sstream.get();
          if (c == ' ' or c == '/') {
            tokens_count++;
          }
          else {
            tokens[tokens_count].append(1, c);
          }
        }

        tokens[tokens_count].pop_back();

        new_triangles.push_back(
          Triangle{
            {
              std::stoi(tokens[0]) - 1,
              std::stoi(tokens[2]) - 1,
              std::stoi(tokens[4]) - 1
            },
            {
              std::stoi(tokens[1]) - 1,
              std::stoi(tokens[3]) - 1,
              std::stoi(tokens[5]) - 1
            },
            { 255, 255, 255 }
          }
        );
      }
    }
  }


  // ~ Copy
  auto Mesh::CopyDataFromVectors(
      std::vector<math::Vec3>& new_verts,
      std::vector<math::Vec2>& new_uv_coords,
      std::vector<Triangle>& new_triangles) -> void {
    // Copy verts data
    verts_count_ = new_verts.size();
    verts_ = new math::Vec3[verts_count_];
    for (int i=0; i<verts_count_; ++i) {
      verts_[i] = new_verts[i];
    }

    // Copy triangles data
    triangles_count_ = new_triangles.size();
    triangles_ = new Triangle[triangles_count_];
    for (int i=0; i<triangles_count_; ++i) {
      triangles_[i] = new_triangles[i];
    }

    // Copy uv coords data
    if (texture_name_[0] == '\0') {
      uv_coords_ = new math::Vec2[0];
      return;
    }

    uv_coords_count_ = new_uv_coords.size();
    uv_coords_ = new math::Vec2[uv_coords_count_];
    for (int i=0; i<uv_coords_count_; ++i) {
      uv_coords_[i] = new_uv_coords[i];
    }
  }
}
