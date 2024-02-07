#pragma once

#include <map>

#include <SDL_mixer.h>

#include "math/vec3.h"

namespace eng::window {

class Window; // forward declaration to make "Window"
              // a friend class of Audio, so nobody else
              // couldn't instantiate it.

class Audio {
friend Window;
 public:
  // Assigns the given value to the "global_volume_" variable.
  // It is used to scale the volume of all played sounds.
  auto SetGlobalVolume(float) -> void;

  // Assigns the given pointers to the "listener_" struct.
  // This values used to scale the pan and the volume of sound
  // played via "PlaySound3D" method.
  auto SetListenerTransform(const math::Vec3*,
                            const math::Vec3*) -> void;

  // Loads a file into a Sound object.
  auto LoadSound(const char *file, const char *name) -> void;

  // Unloads a Sound object by it's name.
  auto UnloadSound(const char*)                      -> void;

  // Plays a sound from the Sound object specified by it's name.
  // Volume, pan and pitch of the played sound can be specified.
  auto PlaySound(const char*, float volume=1.0f,
                 float pan=0.0f, float pitch=1.0f)   -> void;

  // Plays a sound from the Sound object specified by it's name
  // automaticly scaling it's pan and volume depending on
  // sound position, sound range, listener position and 
  // listener rotation.
  auto PlaySound3D(const char*, math::Vec3,
                   float range, float volume=1.0f)   -> void;

 private:
  Audio();
  ~Audio();

  std::map<const char*, Mix_Chunk*> sounds_;

  struct {
    const math::Vec3 *pos;
    const math::Vec3 *rot;
  } listener_;

  float global_volume_ = 1.0f;
};

}

