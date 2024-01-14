#include "gmpl/entity.h"

namespace eng::gmpl {

Entity::Entity()
  : scene(gmpl::Scene::GetInstance()),
    input(sdl::Input::GetInstance()),
    drawer(sdl::Drawer::GetInstance()),
    renderer3d(gfx::Renderer3D::GetInstance()) {}
}
