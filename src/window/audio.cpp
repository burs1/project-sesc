#include "window/audio.h"

namespace eng::sdl {
  
Audio::Audio() {
  Mix_Init(MIX_INIT_OGG);
  Mix_OpenAudio(44100, MIX_DEFAULT_FORMAT, 2, 2048);
}


Audio::~Audio() {
  // Unload all loaded sounds
  for (auto [name, sound] : sounds_) {
    Mix_FreeChunk(sounds_[name]);
  }

  Mix_Quit();
  // Map free
}

// Methods
// ~ Set up
void Audio::SetGlobalVolume(float volume) {
  global_volume_ = volume;
}


// ~ Resources
void Audio::LoadSound(const char* file, const char* name) {
  if (sounds_.contains(name)) {
    throw std::runtime_error("Sound with name \"" + std::string(name) + "\" already has been loaded.");
  }

  sounds_[name] = Mix_LoadWAV(file);

  // Check if sound was successfully loaded
  if (sounds_[name] == NULL) {
    throw std::runtime_error(Mix_GetError());
  }
}


void Audio::UnloadSound(const char* name) {
  Mix_FreeChunk(sounds_[name]);
  sounds_.erase(name);
}


// ~ Audio
void Audio::PlaySound(const char* name, float volume) {
  Mix_Volume(-1, MIX_MAX_VOLUME * global_volume_ * volume);
  Mix_PlayChannel(-1, sounds_[name], 0);
}


}
