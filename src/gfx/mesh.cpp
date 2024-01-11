#include "gfx/mesh.h"

#include <iostream>

namespace eng::gfx {
  Mesh::Mesh(const char *file) {
    LoadFromOBJ(file);
  }


  Mesh::~Mesh() {
    delete[] verts;
    delete[] triangles;
  }


  auto Mesh::LoadFromOBJ(const char *file) -> void {
    std::ifstream in(file);

    // Check if file was successfully loaded
    if (!in.is_open()) {
      throw std::runtime_error("File \"" + std::string(file) + "\"not found");
    }

    // Read file content
    std::vector<math::Vec3> new_verts;
    std::vector<Triangle> new_triangles;

    ReadOBJ(in, new_verts, new_triangles);
    in.close();

    CopyDataFromVectors(new_verts, new_triangles);
   
    // Vectors free
  }


  auto Mesh::ReadOBJ(
      std::ifstream& in,
      std::vector<math::Vec3>& new_verts,
      std::vector<Triangle>& new_triangles) -> void {
    char line[128];
    char junk;

    while (!in.eof()) {
      // Read line
      in.getline(line, 128); 
      std::strstream sstream;
      sstream << line;

      // Parse data
      if (line[0] == 'v') { // Vertex
        math::Vec3 v;
        sstream >> junk >> v.x >> v.y >> v.z;

        new_verts.push_back(v);
      }
      else if (line[0] == 'f') { // Triangle
        int vertsIds[3];
        sstream >> junk >> vertsIds[0] >> vertsIds[1] >> vertsIds[2];
        
        new_triangles.push_back(
          Triangle{
            {
              vertsIds[0] - 1,
              vertsIds[1] - 1,
              vertsIds[2] - 1
            },
            {
              255,
              255,
              255
            }
          }
        );
      }
    }
  }


  auto Mesh::CopyDataFromVectors(
      std::vector<math::Vec3>& new_verts,
      std::vector<Triangle>& new_triangles) -> void {
    // Copy verts data
    verts_count = new_verts.size();
    verts = new math::Vec3[verts_count];
    for (int i=0; i<verts_count; ++i) {
      verts[i] = new_verts[i];
    }

    // Copy triangles data
    triangles_count = new_triangles.size();
    triangles = new Triangle[triangles_count];
    for (int i=0; i<triangles_count; ++i) {
      triangles[i] = new_triangles[i];
    }
 
  }
}
