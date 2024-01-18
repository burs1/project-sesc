#include "gmpl/entity.h"

namespace eng::gmpl {

Entity::Entity()
  : scene(gmpl::Scene::GetInstance()),
    input(window::Input::GetInstance()),
    drawer(window::Drawer::GetInstance()),
    renderer3d(gfx::Renderer3D::GetInstance()) {}
}
