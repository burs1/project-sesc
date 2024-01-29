#include <stdexcept>

#include <SDL.h>
#include <SDL_ttf.h>
#include <SDl_image.h>

#include "window/drawer.h"
#include "logger/logger.h"

namespace eng::window {

Drawer* Drawer::kInstance = nullptr;

// - Static methods -
auto Drawer::GetInstance() -> Drawer* {
  if (kInstance) {
    return kInstance;
  }

  throw std::runtime_error("Trying to get a drawer instance instance when it doesn't exist");
}


auto Drawer::Destroy() -> void {
  if (kInstance) {
    delete kInstance;
    log::Info("Drawer instance destroyed.");
    return;
  }

  throw std::runtime_error("Trying to destroy a drawer instance when it doens't exist");
}


}
