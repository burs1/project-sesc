#include "mesh.h"

#include <iostream>

using namespace std;
using namespace engine::math; 

namespace engine::gfx {
  mesh::mesh(const char *path) {
    _load_from_obj(path);
  }

  mesh::~mesh() {
    faces.clear();
    verts.clear();
  }

  auto mesh::_load_from_obj(const char *path) -> void {
    ifstream in(path);

    if (!in.is_open())
      throw runtime_error("File not found");

    char mode;
    while (in >> mode)
    {
      if (mode == 'f') {
        int vertsIds[3];
        in >> vertsIds[0] >> vertsIds[1] >> vertsIds[2];
        
        faces.push_back(
          face{
            vertsIds[0] - 1, 
            vertsIds[1] - 1, 
            vertsIds[2] - 1
          }
        );
      }
      else if(mode == 'v') {
        vec3 v;
        in >> v.x >> v.y >> v.z;
        verts.push_back(v);
      }
    }

    in.close();
  }
}
