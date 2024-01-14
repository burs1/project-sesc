#pragma once

#include <map>
#include <string>
#include <stdexcept>

#include <SDL_mixer.h>

#include "math/vec3.h"

namespace eng::sdl {

class Audio {
public:
  // Static methods
  static auto Init()        -> void;

  static auto GetInstance() -> Audio*;

  static auto Quit()        -> void;

  // Not copyable
  Audio(const Audio&) = delete;

  Audio operator=(const Audio&) = delete;

  // Not movable
  Audio(Audio&&) = delete;

  Audio& operator=(Audio&&) = delete;

  // Methods
  // ~ Set up
  auto SetGlobalVolume(float)                                         -> void;

  auto SetListenerTransform(const math::Vec3*, const math::Vec3*)     -> void;

  // ~ Resources
  auto LoadSound(const char*, const char*)                            -> void;

  auto UnloadSound(const char*)                                       -> void;

  // ~ Audio
  auto PlaySound(const char*, float volume=1.0f)                      -> void;

  auto PlaySound3D(const char*, math::Vec3, float, float volume=1.0f) -> void;

private:
  Audio();

  ~Audio();

  // Vars
  std::map<const char*, Mix_Chunk*> sounds_;

  struct { const math::Vec3* pos, rot; } listener;

  float global_volume_ = 1.0f;

  static Audio* kInstance;

};

}
