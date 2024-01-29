#include <map>
#include <string>
#include <stdexcept>

#include <SDL_mixer.h>

#include "math/vec3.h"
#include "math/vec2.h"
#include "window/audio.h"
#include "logger/logger.h"

namespace eng::window {

Audio* Audio::kInstance = nullptr;

// - Static methods -
auto Audio::Create() -> void {
  if (not kInstance) {
    kInstance = new Audio();
    log::Info("Audio system instance created");
    return;
  }

  throw std::runtime_error("Trying to create an audio system instance when it is already created");
}


auto Audio::GetInstance() -> Audio* {
  if (kInstance) { return kInstance; }

  throw std::runtime_error("Trying to get an audio system instance when it doesn't exist");
}


auto Audio::Destroy() -> void {
  if (kInstance) {
    delete kInstance;
    log::Info("Audio system destroyed");
    return;
  }

  throw std::runtime_error("Trying to destroy an audio system when it doens't exist");
}


// - Methods -
void Audio::SetGlobalVolume(float volume) {
  global_volume_ = volume;
}


void Audio::LoadSound(const char* file, const char* sound_name) {
  if (sounds_.contains(sound_name)) {
    throw std::runtime_error("Sound with name \"" + std::string(sound_name) + "\" already has been loaded.");
  }

  sounds_[sound_name] = Mix_LoadWAV(file);

  // Check if sound was successfully loaded
  if (sounds_[sound_name] == NULL) {
    throw std::runtime_error(Mix_GetError());
  }
}


void Audio::UnloadSound(const char* sound_name) {
  Mix_FreeChunk(sounds_[sound_name]);
  sounds_.erase(sound_name);
}


void Audio::PlaySound(
    const char* sound_name,
    float volume,
    float pan,
    float pitch) {
  int channel = Mix_PlayChannel(-1, sounds_[sound_name], 0);
  Mix_Volume(channel, MIX_MAX_VOLUME * global_volume_ * volume);
  Mix_SetPanning(
    channel,
    (pan > 0 ? 255 * (1 - pan) : 255),
    (pan < 0 ? 255 * (1 + pan) : 255));
}

auto Audio::PlaySound3D(
    const char* sound_name,
    math::Vec3 pos,
    float range,
    float volume) -> void {
  int channel = Mix_PlayChannel(-1, sounds_[sound_name], 0);
  Mix_Volume(channel, MIX_MAX_VOLUME * global_volume_ * volume);
  Mix_SetPosition(
    channel,
    math::Vec2::AngleBetween(
      math::Vec2(0.0f, 1.0f),
      math::Vec2(pos.x, pos.z)) - listener_.rot->y, 
    255 * ((pos - *listener_.pos).Magnitude() / range));
}


// Constructor
Audio::Audio() {
  Mix_Init(MIX_INIT_OGG);
  Mix_OpenAudio(44100, MIX_DEFAULT_FORMAT, 2, 2048);
}


// Destructor
Audio::~Audio() {
  // Unload all loaded sounds
  for (auto [name, sound] : sounds_) {
    Mix_FreeChunk(sounds_[name]);
  }

  Mix_Quit();
  // Map free
}


}
