#pragma once

#include <map>
#include <string>
#include <stdexcept>

#include <SDL_mixer.h>

namespace eng::sdl {

class Audio {
public:
  Audio();

  ~Audio();

  // Not copyable
  Audio(const Audio&) = delete;

  Audio operator=(const Audio&) = delete;

  // Not movable
  Audio(Audio&&) = delete;

  Audio& operator=(Audio&&) = delete;

  // Methods
  // ~ Set up
  auto SetGlobalVolume(float)                    -> void;

  // ~ Resources
  auto LoadSound(const char*, const char*)       -> void;

  auto UnloadSound(const char*)                  -> void;

  // ~ Audio
  auto PlaySound(const char*, float volume=1.0f) -> void;

private:
  // Vars
  std::map<const char*, Mix_Chunk*> sounds_;

  float global_volume_ = 1.0f;

};

}
