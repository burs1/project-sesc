#include "window/drawer.h"

namespace eng::window {

Drawer* Drawer::kInstance = nullptr;

// Static methods
auto Drawer::GetInstance() -> Drawer* {
  if (kInstance) {
    return kInstance;
  }
  throw std::runtime_error("Drawer is offline.");
}


auto Drawer::Quit() -> void {
  if (kInstance) {
    delete kInstance;
    return;
  }
  throw std::runtime_error("Drawer is already offline.");
}


}
